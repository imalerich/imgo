CFLAGS = -std=c++11 -Wall -g -c
OBJ = GtpEngine.o GtpPlayer.o GtpVertex.o GtpCommand.o
LIB = ../../lib
OUT = $(LIB)/libimgtp.a

imgtp.a: $(OBJ)
	[ -d $(LIB) ] || mkdir $(LIB)
	ar rvs $(OUT) $(OBJ)

GtpEngine.o: GtpEngine.hpp GtpEngine.cpp
	g++ $(CFLAGS) GtpEngine.cpp

GtpPlayer.o: GtpPlayer.hpp GtpPlayer.cpp
	g++ $(CFLAGS) GtpPlayer.cpp

GtpVertex.o: GtpVertex.hpp GtpVertex.cpp
	g++ $(CFLAGS) GtpVertex.cpp
	
GtpCommand.o: GtpCommand.hpp GtpCommand.cpp
	g++ $(CFLAGS) GtpCommand.cpp

clean:
	rm -rf *.o
	rm -rf $(OUT)
