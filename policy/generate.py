from os.path import isfile, join
from os import listdir
import numpy as np
import random
import caffe
import lmdb
import sgf

# Import our python utilities.
import sys
sys.path.append('../python')
imgo = __import__('imgo')

# Directoryies and Strings.
PATH = '../data/games/'

TRAIN_DB = "../data/lmdb/lmdb_policy_train"
TEST_DB = "../data/lmdb/lmdb_policy_test"

# List all of the files in that record.
# This doesn't check if they are all .sgf, but they should be.
FILES = [f for f in listdir(PATH) if isfile(join(PATH, f))]
# Set this value if you want to limit the number of records added to the database.
NUM_RECORDS = len(FILES)
# But don't mess with these.
NUM_TRAIN = int(NUM_RECORDS * 0.75)
NUM_TEST = NUM_RECORDS - NUM_TRAIN
# How many snapshots should we take of each game?
SAMPLES_PER_GAME = 81

GAME_BYTES = imgo.CHANNELS * imgo.BOARD_SIZE**2  * np.dtype(np.int8).itemsize
TRAIN_MAP_SIZE = 10 * NUM_TRAIN * GAME_BYTES * SAMPLES_PER_GAME
TEST_MAP_SIZE = 10 * NUM_TEST * GAME_BYTES * SAMPLES_PER_GAME

# Create and open an LMDB Database for both our training and test records.
train_env = lmdb.open(TRAIN_DB, map_size=TRAIN_MAP_SIZE)
test_env = lmdb.open(TEST_DB, map_size=TEST_MAP_SIZE)

# The total number of entries added to the training and test databases, 
# this is slightly random, but should end up being about NUM_RECORDS * SAMPLES_PER_GAME.
NUM_ENTRIES = 0

with train_env.begin(write=True) as train_txn, test_env.begin(write=True) as test_txn:

    # Loop through a number of game records.
    for g in range(0, NUM_RECORDS):

        # Pick a random record from our list of files.
        filename = random.choice(FILES)
        FILES.remove(filename)
        filename = PATH + filename

        # Decide whether to add this to the train or to the test database.
        add_to_train = (g < NUM_TRAIN)

        # Open the file and parse the record.
        with open(filename, 'r') as f:

            r = f.read()

            # Next up we need to build the array representing the game board.
            # This will be a 9x9 board with 2 channel per location.
            # White will be channel 0, Black will be channel 1.
            board = np.zeros((imgo.CHANNELS, imgo.BOARD_SIZE, imgo.BOARD_SIZE), dtype=np.uint8)
            game = sgf.parse(r).children[0]

            txn = train_txn if add_to_train else test_txn
            db_name = TRAIN_DB if add_to_train else TEST_DB

            # Loop through each move of the game.
            for node in game.rest:
                print('Adding move from ' + str(filename) + ' to ' + db_name)
                nxt = imgo.nodeToIndex(node)
                imgo.recordEntry(g, board, nxt, txn)
                NUM_ENTRIES += 1
                imgo.addNodeToGame(board, node)

    print('SUCCESS! Generated ' + str(NUM_ENTRIES) + ' entries.')
