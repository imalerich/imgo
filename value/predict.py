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
MODEL = 'imgo_value_net_deploy.prototxt'
PRETRAINED = 'imgo_value_iter_10000.caffemodel'

# List all of the files in that record.
# This doesn't check if they are all .sgf, but they should be.
FILES = [f for f in listdir(PATH) if isfile(join(PATH, f))]
# Set this value if you want to limit the number of records added to the database.
NUM_RECORDS = len(FILES)

caffe.set_mode_gpu()
net = caffe.Net(MODEL, PRETRAINED, caffe.TEST)

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
        score = imgo.gameWinner(r)
        board = np.zeros((imgo.CHANNELS, imgo.BOARD_SIZE, imgo.BOARD_SIZE), dtype=np.uint8)
        game = sgf.parse(r).children[0]

        # Loop through each move of the game.
        for node in game.rest:
            imgo.addNodeToGame(board, node)

        # Run the game through our network.
        net.blobs['data'].data[...] = board
        net.forward()
        pr = net.blobs['loss'].data[0]

        TOTAL += 1
        if score == pr.argmax():
            CORRECT += 1

# How well did we do?
print(str(CORRECT) + '/' + str(TOTAL))
