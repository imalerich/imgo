CFLAGS = -std=c++1y -Wall -g -c
OBJ = Engine.o Player.o Vertex.o Command.o Argument.o Move.o
LIB = ../../lib
OUT = $(LIB)/libimgtp.a

imgtp.a: $(OBJ)
	[ -d $(LIB) ] || mkdir $(LIB)
	ar rvs $(OUT) $(OBJ)

Engine.o: Engine.hpp Engine.cpp Move.hpp Command.hpp Defaults
	g++ $(CFLAGS) Engine.cpp

Player.o: Player.hpp Player.cpp
	g++ $(CFLAGS) Player.cpp

Vertex.o: Vertex.hpp Vertex.cpp
	g++ $(CFLAGS) Vertex.cpp
	
Command.o: Command.hpp Command.cpp
	g++ $(CFLAGS) Command.cpp

Argument.o: Argument.hpp Argument.cpp
	g++ $(CFLAGS) Argument.cpp

Move.o: Argument.hpp Player.hpp Vertex.hpp Move.hpp Move.cpp
	g++ $(CFLAGS) Move.cpp

Defaults: Argument.hpp Command.hpp Vertex.hpp

clean:
	rm -rf *.o
	rm -rf $(OUT)
