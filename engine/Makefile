CFLAGS = -std=c++1y -Wall -g -c -I ./
LFLAGS = -lm -L ../lib
OBJ = imgo.o
LIB = -limgtp -lboost_system

imgo: $(OBJ)
	[ -d ../bin ] || mkdir ../bin
	g++ -o ../bin/imgo $(LFLAGS) $(OBJ) $(LIB)

imgo.o: imgo.cpp
	g++ $(CFLAGS) imgo.cpp

run:
	../bin/imgo

clean:
	rm -rf *.o
	rm -rf ../bin/imgo
