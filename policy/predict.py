from os.path import isfile, join
from os import listdir
import numpy as np
import random
import lmdb
import caffe
import sgf
import re

# Import our python utilities.
import sys
sys.path.append('../python')
imgo = __import__('imgo')

# Directoryies and Strings.
PATH = '../data/games/'
MODEL = 'imgo_policy_net_deploy.prototxt'
WHITE_PRETRAINED = '../net/imgo_white_policy_iter_10000.caffemodel'
BLACK_PRETRAINED = '../net/imgo_black_policy_iter_10000.caffemodel'

# List all of the files in that record.
# This doesn't check if they are all .sgf, but they should be.
FILES = [f for f in listdir(PATH) if isfile(join(PATH, f))]
# Set this value if you want to limit the number of records added to the database.
NUM_RECORDS = len(FILES)

caffe.set_mode_gpu()
white_net = caffe.Net(MODEL, WHITE_PRETRAINED, caffe.TEST)
black_net = caffe.Net(MODEL, BLACK_PRETRAINED, caffe.TEST)

# How many of our predictions did we predict correctly?
CORRECT = 0
# How many predictions did we make?
TOTAL = 0

for g in range(0, NUM_RECORDS):

    # Pick a random record from our list of files.
    filename = random.choice(FILES)
    FILES.remove(filename)
    filename = PATH + filename

    with open(filename, 'r') as f: 

        r = f.read()
        board = np.zeros((imgo.CHANNELS, imgo.BOARD_SIZE, imgo.BOARD_SIZE), dtype=np.uint8)
        game = sgf.parse(r).children[0]

        # Loop through each move of the game.
        for node in game.rest:
            # Run the game through our network.
            net = black_net if imgo.isNodeBlack(node) else white_net
            net.blobs['data'].data[...] = board
            net.forward()
            pr = net.blobs['loss'].data[0]

            # Find the 5 moves we think are most likely.
            moves = []
            for i in range(0, 10):
                idx = pr.argmax()
                moves.append(idx)
                pr[idx] = 0 # Don't take moves more than once.

            # Check if the actual move was predicted.
            TOTAL += 1
            if imgo.nodeToIndex(node) in moves:
                CORRECT += 1

            # Update the game board.
            imgo.addNodeToGame(board, node)

# How well did we do?
print(str(CORRECT) + '/' + str(TOTAL))
