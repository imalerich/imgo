#! /bin/bash

# The current index of the game we are currently recording.
INDEX=0
# Directory in which to store all games records.
GAMES_DIR="games"
# Number of games to play per gnugo level.
GAMES_PER_LEVEL=100
# Minimum level of gnugo to simulate.
MIN_LEVEL=1
# Maximum level of gnugo to simulate.
MAX_LEVEL=10
# The total number of games simulated will be (MAX_LEVEL-MIN_LEVEL+1) * GAMES_PER_LEVEL.

# Check if the games directory exists, if it does, wipe it.
if [ -d $GAMES_DIR ]; then
	rm -rf games
fi

# Then create a new empty directory.
mkdir $GAMES_DIR

# For each available level
for LEVEL in $(seq $MIN_LEVEL $MAX_LEVEL)
do
	# Run a number of simulations.
	for i in $(seq 1 $GAMES_PER_LEVEL)
	do
		# Run a simulation through gnugo adn output the result as an sgf in GAMES_DIR.
		FILENAME="$GAMES_DIR/$INDEX.sgf"
		gnugo --boardsize 9 --never-resign --komi 0 --benchmark 81 --level $LEVEL --outfile $FILENAME
		# Increment the current game index.
		INDEX=$((INDEX+1))
	done
done
