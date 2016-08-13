import numpy as np
import random
import lmdb
import caffe
import sgf
import re

BOARD_SIZE = 9
CHANNELS = 2

caffe.set_mode_gpu()
MODEL = 'imgo_value_net_deploy.prototxt'
PRETRAINED = 'imgo_value_iter_10000.caffemodel'
net = caffe.Net(MODEL, PRETRAINED, caffe.TEST)

CORRECT=0
TOTAL=0

NUM_RECORDS = 1000
for g in range(0, NUM_RECORDS):

    filename = 'games/' + str(g) + '.sgf'
    with open(filename, 'r') as f: 

        r = f.read()
        res = re.search('RE\[[BW]\+\d+.\d\]', r).group(0)[3:-1]
        board = np.zeros((1, CHANNELS, BOARD_SIZE, BOARD_SIZE), dtype=np.uint8)
        game = sgf.parse(r).children[0]
        if res[0] == 'B':
            score = 0
        else:
            score = 1

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
                board[0][channel][x][y] = 1

        net.blobs['data'].data[...] = board
        net.forward()
        pr = net.blobs['loss'].data[0]

        TOTAL+=1
        if score == pr.argmax():
            CORRECT+=1

print(str(CORRECT) + '/' + str(TOTAL))
