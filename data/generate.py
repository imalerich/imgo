import numpy as np
import random
import lmdb
import caffe
import sgf
import re

# How many records to print out?
# This should be less than or equal to the number of game records
# generated by the simulate.sh script.
NUM_RECORDS = 1000
# Should be the same BOARD_SIZE as found in generate.sh that was used to generate
# the games directory.
BOARD_SIZE = 9
# How many channels to use (1 for each player).
CHANNELS = 2

# We want to print the total number of records generated at the end of this script
NUM_ENTRIES = 0

# Create the LMDB database for storing our records.
DB_NAME = "lmdb_records_" + str(NUM_RECORDS)
MAP_SIZE = 10 * NUM_RECORDS * CHANNELS * BOARD_SIZE * BOARD_SIZE * np.dtype(np.int8).itemsize
env = lmdb.open(DB_NAME, map_size=MAP_SIZE)
with env.begin(write=True) as txn:

    # Record a game entry to the lmdb database.
    def recordentry(game, board, score, txn):
        datum = caffe.proto.caffe_pb2.Datum()
        datum.channels = board.shape[0]
        datum.height = board.shape[1]
        datum.width = board.shape[2]
        datum.data = board.tobytes()
        datum.label = score
        str_id = '{:08}'.format(game)
        txn.put(str_id, datum.SerializeToString())
        return

    # Loop through a number of game records.
    for g in range(0, NUM_RECORDS):

        # Open a file for parsing.
        game_num = int(random.random() * 1000) % 1000
        filename = 'games/' + str(game_num) + '.sgf'
        with open(filename, 'r') as f:

            print('Adding ' + str(filename) + ' to lmdb...')

            # Use a regular expression to try and find the score of the game.
            # We will compute a single value to use as a label in our training and validation sets.
            # Positive will be a black win and negative a white win.
            # This score will ignore the constant komi modifier.
            r = f.read()
            res = re.search('RE\[[BW]\+\d+.\d\]', r).group(0)[3:-1]
            kom = float(re.search('KM\[\d+.\d\]', r).group(0)[3:-1])

            # Check the winner, then rip that label off of the result, and calculate the score.
            win = res[0]
            score = float(res[2:]) # Score needs to be a float until we remove komi.
            if win == 'B':
                # score += kom
                score = 0
            else:
                # score = -(score - kom)
                score = 1
            score = int(score)

            # Next up we need to build the array representing the game board.
            # This will be a 9x9 board with 2 channel per location.
            # White will be channel 0, Black will be channel 1.
            board = np.zeros((CHANNELS, BOARD_SIZE, BOARD_SIZE), dtype=np.uint8)
            game = sgf.parse(r).children[0]

            # Loop through each move of the game.
            for node in game.rest:
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

                # TODO - We need to account for this in the database size.
                # Include random entries from this game.
                # if random.random() < 0.3:
                #    NUM_ENTRIES += 1
                #    recordentry(g, board, score, txn)

            recordentry(NUM_ENTRIES, board, score, txn)
            NUM_ENTRIES += 1

    print('SUCCESS: ' + str(NUM_ENTRIES) + ' records generated')
