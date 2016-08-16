CFLAGS = -std=c++1y -Wall -g -c
OBJ = Engine.o Player.o Vertex.o Command.o
LIB = ../../lib
OUT = $(LIB)/libimgtp.a

imgtp.a: $(OBJ)
	[ -d $(LIB) ] || mkdir $(LIB)
	ar rvs $(OUT) $(OBJ)

Engine.o: Engine.hpp Engine.cpp Move Command.o
	g++ $(CFLAGS) Engine.cpp

Player.o: Player.hpp Player.cpp
	g++ $(CFLAGS) Player.cpp

Vertex.o: Vertex.hpp Vertex.cpp
	g++ $(CFLAGS) Vertex.cpp
	
Command.o: Command.hpp Command.cpp
	g++ $(CFLAGS) Command.cpp

Move: Argument Player.o Vertex.o

Argument: Argument.hpp

clean:
	rm -rf *.o
	rm -rf $(OUT)
