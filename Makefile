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
LDFLAGS =
INCDIR = -Iinclude

TIMER = 0
TCOMP =

SHOW_ALLOC = 0
SHOW_ALLOCCOMP =

ifeq ($(TIMER), 1)
#Timer compilation is ON
TCOMP = -DTIME=1
endif

ifeq ($(SHOW_ALLOC), 1)
SHOW_ALLOCCOMP = -DSHOW_ALLOC=1
endif

all : $(EXE)

$(EXE) : $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5)
	g++ -o $(EXE) $(LDFLAGS) $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5)

$(OBJ1) : $(SRC1) $(HFILES)
	g++ $(INCDIR) -c $(CFLAGS) $(TCOMP) $(SHOW_ALLOCCOMP) $(SRC1) -o $(OBJ1)

$(OBJ2) : $(SRC2) $(HFILES)
	g++ $(INCDIR) -c $(CFLAGS) $(TCOMP) $(SHOW_ALLOCCOMP) $(SRC2) -o $(OBJ2)

$(OBJ3) : $(SRC3) $(HFILES)
	g++ $(INCDIR) -c $(CFLAGS) $(TCOMP) $(SHOW_ALLOCCOMP) $(SRC3) -o $(OBJ3)

$(OBJ4) : $(SRC4) $(HFILES)
	g++ $(INCDIR) -c $(CFLAGS) $(TCOMP) $(SHOW_ALLOCCOMP) $(SRC4) -o $(OBJ4)

$(OBJ5) : $(SRC5) $(HFILES)
	g++ $(INCDIR) -c $(CFLAGS) $(TCOMP) $(SHOW_ALLOCCOMP) $(SRC5) -o $(OBJ5)

debug: CFLAGS = -g -Wall  -DDEBUG=1
debug: all

cleanup:
	@echo "Deleting object files..."
	rm -f src/*.o
