import numpy as np
import caffe
import lmdb
import sgf
import re

# Size of the game board.
BOARD_SIZE = 9
# Number of channels to use (1 for each player).
CHANNELS = 2

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
        board[channel][x][y] = 1

    return

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
