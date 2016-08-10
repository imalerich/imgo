# imgo

## Requirements

This project requires a working [caffe](https://github.com/BVLC/caffe) installation (with python).
GnuGo is required for the simulate.sh bash script for generating game data.
Alongside caffe's existing python dependencies, jtauber's [sgf](https://github.com/jtauber/sgf) parser is also required.

## simulate.sh

Runs a number of simulations of Go by having GnuGo playing itself. Each game is saved as a .sgf file in the data/games/ directory. The default board size is 9x9, with 100 games for each GnuGo level, resulting in a total of 1000 games. Each of these values may be changed by editing the simulate.sh script.

## generate.python

Generates the lmdb databases that will be used to train the networks in caffe. NUM_RECORDS may be changed in this script if you do not want to use all of the games created by the simulate.sh script, however this value should NOT exceed the number of records available. Further the BOARD_SIZE parameter should match the value found in simulate.sh.
