CFLAGS = -std=c++1y -Wall -g -c -I./ -I$(CUDA)/include/ -I$(CAFFE_DIST)/include/
LFLAGS = -lm -L ../lib -L $(CAFFE_DIST)/lib/
OBJ = imgo.o
LIB = -limgtp -lboost_system -lcaffe

imgo: $(OBJ)
	[ -d ../bin ] || mkdir ../bin
	g++ -o ../bin/imgo $(OBJ) $(LFLAGS) $(LIB)

imgo.o: imgo.cpp
	g++ $(CFLAGS) imgo.cpp

run:
	../bin/imgo

clean:
	rm -rf *.o
	rm -rf ../bin/imgo
