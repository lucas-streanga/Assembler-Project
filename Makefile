SRC1 = src/main.cpp
SRC2 = src/error_handler.cpp
SRC3 = src/init.cpp
SRC4 = src/Virtual_memory.cpp
SRC5 = src/decode.cpp
OBJ1 = $(SRC1:.cpp=.o)
OBJ2 = $(SRC2:.cpp=.o)
OBJ3 = $(SRC3:.cpp=.o)
OBJ4 = $(SRC4:.cpp=.o)
OBJ5 = $(SRC5:.cpp=.o)

EXE = Assembler

HFILES = include/asm_defines.h include/error_handler.h include/init.h include/decode.h
CFLAGS = -O2
INCDIR = -Iinclude

all : $(EXE)

$(EXE) : $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5)
	g++ -o $(EXE) $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5)

$(OBJ1) : $(SRC1) $(HFILES)
	g++ $(INCDIR) -c $(CFLAGS) $(SRC1) -o $(OBJ1)

$(OBJ2) : $(SRC2) $(HFILES)
	g++ $(INCDIR) -c $(CFLAGS) $(SRC2) -o $(OBJ2)

$(OBJ3) : $(SRC3) $(HFILES)
	g++ $(INCDIR) -c $(CFLAGS) $(SRC3) -o $(OBJ3)

$(OBJ4) : $(SRC4) $(HFILES)
	g++ $(INCDIR) -c $(CFLAGS) $(SRC4) -o $(OBJ4)

$(OBJ5) : $(SRC5) $(HFILES)
	g++ $(INCDIR) -c $(CFLAGS) $(SRC5) -o $(OBJ5)

debug: CFLAGS = -g -Wall -DDEBUG=1
debug: all

cleanup:
	@echo "Deleting object files..."
	rm -f src/*.o
