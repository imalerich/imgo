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

# The total number of entries added to the training and test databases, 
# this is slightly random, but should end up being about NUM_RECORDS * SAMPLES_PER_GAME.
NUM_ENTRIES = 0

# List of all entries, we will fill this up, randomize it, and then 
# add the contents to our database.
ENTRIES = []

# Generate all of the records, these will need to be shuffled before adding 
# them to our database.
for g in range(0, NUM_RECORDS):

    # Pick a random record from our list of files.
    filename = random.choice(FILES)
    FILES.remove(filename)
    filename = PATH + filename

    # Open the file and parse the record.
    with open(filename, 'r') as f:

        r = f.read()

        # Next up we need to build the array representing the game board.
        # This will be a 9x9 board with 2 channel per location.
        # White will be channel 0, Black will be channel 1.
        board = np.zeros((imgo.CHANNELS, imgo.BOARD_SIZE, imgo.BOARD_SIZE), dtype=np.uint8)
        game = sgf.parse(r).children[0]

        # Loop through each move of the game.
        for node in game.rest:
            nxt = imgo.nodeToIndex(node)
            if nxt != 81:
                print('Generating entry from ' + str(filename))
                ENTRIES.append((np.fliplr(board), nxt))
                ENTRIES.append((np.flipud(board), nx))
                for r in range(0,4):
                    ENTRIES.append((np.rot90(board, r), nxt))
                NUM_ENTRIES += 1
            imgo.addNodeToGame(board, node)


# Shuffle all of the records and create our databases.
print('Shuffling the data')
np.random.shuffle(ENTRIES)
NUM_TRAIN = int(len(ENTRIES) * 0.75)
NUM_TEST = len(ENTRIES) - NUM_TRAIN

GAME_BYTES = imgo.CHANNELS * imgo.BOARD_SIZE**2  * np.dtype(np.int8).itemsize
TRAIN_MAP_SIZE = 10 * NUM_TRAIN * GAME_BYTES
TEST_MAP_SIZE = 10 * NUM_TEST * GAME_BYTES

# Create and open an LMDB Database for both our training and test records.
train_env = lmdb.open(TRAIN_DB, map_size=TRAIN_MAP_SIZE)
test_env = lmdb.open(TEST_DB, map_size=TEST_MAP_SIZE)

# Add all the records we generated to these databases.
with train_env.begin(write=True) as train_txn, test_env.begin(write=True) as test_txn:

    print('Adding records to LMDB.')
    for i in range(len(ENTRIES)):
        # Decide whether to add this to the train or to the test database.
        add_to_train = (i < NUM_TRAIN)
        txn = train_txn if add_to_train else test_txn
        db_name = TRAIN_DB if add_to_train else TEST_DB
        imgo.recordEntry(g, ENTRIES[i][0], ENTRIES[i][1], txn)

    print('SUCCESS! Generated ' + str(NUM_ENTRIES) + ' entries.')
