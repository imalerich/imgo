import numpy as np
import caffe
import lmdb
import sgf
import re

# Size of the game board.
BOARD_SIZE = 9
# Number of Channels to use, the data stored in each channel is as follows.
# 0 White Stone
# 1 Black Stones
# 2 Turns Since Stone
# 3 Number of Liberties
CHANNELS = 4

# Returns the winner of the game represented by
# r (the contents of an sgf file) as an integer,
# where { Black: 0, White: 1 }.
def gameWinner(r):
    # Use a regular expression to try and find the score of the game.
    res = re.search('RE\[[BW]\+\d+.\d\]', r).group(0)[3:-1]
    return 0 if res[0] == 'B' else 1

# Convert a node to a single index that can be used as a label for training
# the policy network. The positions will be read from top to bottom, left to right.
# So the position (a,a) will be mapped to 0, and (i,i) will be 80 (on a 9,9 board).
# A pass will be the next available integer (81 in this case, or BOARD_SIZE^2).
def nodeToIndex(node):
    if 'W' in node.properties:
        pos = node.properties['W'][0]
    elif 'B' in node.properties:
        pos = node.properties['B'][0]

    if len(pos) >= 2:
        x = ord(pos[0]) - ord('a')
        y = ord(pos[1]) - ord('a')
        return y * BOARD_SIZE + x
    else:
        return BOARD_SIZE**2


# Returns if this current node is to be played by the black player.
def isNodeBlack(node):
    return ('B' in node.properties)

# Record a game entry to the lmdb database.
def recordEntry(game, board, label, txn):
    datum = caffe.proto.caffe_pb2.Datum()
    datum.channels = board.shape[0]
    datum.height = board.shape[1]
    datum.width = board.shape[2]
    datum.data = board.tobytes()
    datum.label = label
    str_id = '{:08}'.format(game)
    txn.put(str_id, datum.SerializeToString())
    return

# For the given board, add the move stored by the
# sgf node.
def addNodeToGame(board, node):
    # Find the move (black or white) and position
    if 'W' in node.properties:
        channel = 0
        pos = node.properties['W'][0]
    elif 'B' in node.properties:
        channel = 1
        pos = node.properties['B'][0]

    # Place the move onto the board.
    if len(pos) >= 2:
        x = ord(pos[0]) - ord('a')
        y = ord(pos[1]) - ord('a')
        board[channel][y][x] = 1
        procCapturesOnChannel(board, invertPlayerChannel(channel))

        # Process any extra information we are storing in our board.
        addAndIncrementCounter(board, y, x)
        rebuildLiberties(board)

    return

# Increments the turn counter (layer 3) and removes captured stones.
# Then add a turn counter for the newly added stone (given by the position x, y).
def addAndIncrementCounter(board, x, y):
    for y in range(0, BOARD_SIZE):
        for x in range(0, BOARD_SIZE):
            if board[0][x][y] == 0 and board[0][y][x] == 0:
                board[2][x][y] = 0 # Stone was captured
            if board[2][x][y] > 0:
                board[2][x][y] += 1 # Increment the turn counter

    board[2][x][y] = 1
    return

# Build the liberty counter for the board (layer 4).
def rebuildLiberties(board):
    for y in range(0, BOARD_SIZE):
        for x in range(0, BOARD_SIZE):
            board[3][x][y] = numberOfLiberties(board, x, y)
    return

# Remove captured stones from the board, only stones of from the given
# channel will be removed, thus this method should be called after placing
# each node, but with the opposite channel of the played node.
def procCapturesOnChannel(board, channel):
    for y in range(0, BOARD_SIZE):
        for x in range(0, BOARD_SIZE):
            checkCaptureOnNode(board, channel, x, y, 0)

    for y in range(0, BOARD_SIZE):
        for x in range(0, BOARD_SIZE):
            board[0][x][y] = 0 if board[0][x][y] == 0 else 1
            board[1][x][y] = 0 if board[1][x][y] == 0 else 1

    return


# Check if the given node, defined by the x, y coordinate in the given channel
# should be captured. If so that node will be removed from the board, this will
# recurse through that stones local group and update them as well. If no stone
# exists at the given position (or one was already removed) this method does nothing.
# This method will modify the board on the given channel with the following parameters:
# 0 : No Stone
# 1 : Unchecked Stone
# 2 : Checked Stone (Alive)
# 3 : Checked but in question (Don't recheck on recursion).
def checkCaptureOnNode(board, channel, x, y, depth):
    if board[channel][x][y] != 1:
        # This stone has already been checked, return the checked value.
        return board[channel][x][y]

    numLiberties = numberOfLiberties(board, x, y)
    neighbors = friendlyNeighbors(board, channel, x, y)

    if numLiberties > 0:
        # Liberties still remain, we are for sure alive.
        board[channel][x][y] = 2
        # Because we are alive, go ahead and set all of our neighbors to alive as well.
        for n in neighbors:
            board[channel][n[0]][n[1]] = 2
        return 2

    if len(neighbors) == 0:
        # No liberties and no neighbors, we are for sure dead.
        board[channel][x][y] = 0
        return 0

    # We have neighbors, but no liberties, our life depends on them.
    # Loop through each of our neighbors, we are alive if at least
    # one of them is alive, this means they MUST return 2.
    # If they return 0 that means they died in their own check.
    # Set ourself to 3, that way our neighbors won't try and check us
    # and we'll get caught in an infinte loop.
    board[channel][x][y] = 3
    alive = 'No' # Assumed to be dead until proven otherwise
    for n in neighbors:
        # We need to check if that node is alive (base case handled above).
        checkCaptureOnNode(board, channel, n[0], n[1], depth+1)
        if board[channel][n[0]][n[1]] == 2:
            alive = 'Yes'
        elif board[channel][n[0]][n[1]] == 3 and alive == 'No':
            alive = 'Maybe'

    if alive == 'Yes':
        # At least one neighbor is alive, therefore we are alive.
        board[channel][x][y] = 2
        # Some of our neighbors might have thought they died, tell them they are alive.
        for n in neighbors:
            board[channel][n[0]][n[1]] = 2
        return 2
    elif alive == 'No' or depth == 0:
        killallChildren(board, channel, x, y)
        return 0
    else: # We might be alive.
        board[channel][x][y] = 3
        return 3

# Recursively kill all of our children.
def killallChildren(board, channel, x, y):
    board[channel][x][y] = 0
    neighbors = friendlyNeighbors(board, channel, x, y)
    for n in neighbors:
        killallChildren(board, channel, n[0], n[1])
    return

def friendlyNeighbors(board, channel, x, y):
    neighbors = []
    if x > 0 and board[channel][x-1][y] != 0:
        neighbors.append((x-1,y))
    if x+1 < BOARD_SIZE and board[channel][x+1][y] != 0:
        neighbors.append((x+1,y))
    if y > 0 and board[channel][x][y-1] != 0:
        neighbors.append((x,y-1))
    if y+1 < BOARD_SIZE and board[channel][x][y+1] != 0:
        neighbors.append((x,y+1))
    return neighbors

# Returns the number of liberties of the given position.
# This method only considers open spaces liberties, friendly stones
# are consider to cover the input stones liberties.
def numberOfLiberties(board, x, y):
    liberties = 0
    if x > 0 and isPosOpen(board,x-1,y):
        liberties += 1
    if x+1 < BOARD_SIZE and isPosOpen(board,x+1,y):
        liberties += 1
    if y > 0 and isPosOpen(board,x,y-1):
        liberties += 1
    if y+1 < BOARD_SIZE and isPosOpen(board,x,y+1):
        liberties += 1
    return liberties

# True if the given position on the board is open, false otherwise.
def isPosOpen(board, x, y):
    return board[0][x][y] == 0 and board[1][x][y] == 0

# Given a players channel, return the other players channel.
def invertPlayerChannel(channel):
    return abs(channel-1)

# Print a little bit more user friendly board that we can output to the standard out.
def printBoard(board):
    out = np.zeros((1, BOARD_SIZE, BOARD_SIZE), dtype=np.uint8)
    out += board[0]
    out += (board[1] * 2)
    print(out)
    print('')
    return
