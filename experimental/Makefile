SRC1 = src/main.cpp
SRC2 = src/helper.cpp
SRC3 = src/lookup_tables.cpp
SRC4 = src/driver.cpp

OBJ1 = $(SRC1:.cpp=.o)
OBJ2 = $(SRC2:.cpp=.o)
OBJ3 = $(SRC3:.cpp=.o)
OBJ4 = $(SRC4:.cpp=.o)

ASM_PARSER = parser/asm_parser

LEX    = flex
YACC   = bison

EXE = Assembler

HFILES = include/*.h parser/driver.h
CFLAGS = -O2
LDFLAGS =
INCDIR = -Iinclude -Iparser

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

$(EXE) : $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(ASM_PARSER).tab.o $(ASM_PARSER).yy.o
	g++ -o $(EXE) $(LDFLAGS) $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(ASM_PARSER).tab.o $(ASM_PARSER).yy.o
	$(MAKE) -C parser cleanup

$(OBJ1) : $(SRC1) $(HFILES)  $(ASM_PARSER).tab.o $(ASM_PARSER).yy.o
	g++ $(INCDIR) -c $(CFLAGS) $(TCOMP) $(SHOW_ALLOCCOMP) $(SRC1) -o $(OBJ1)

$(OBJ2) : $(SRC2) $(HFILES)
	g++ $(INCDIR) -c $(CFLAGS) $(TCOMP) $(SHOW_ALLOCCOMP) $(SRC2) -o $(OBJ2)

$(OBJ3) : $(SRC3) $(HFILES)
	g++ $(INCDIR) -c $(CFLAGS) $(TCOMP) $(SHOW_ALLOCCOMP) $(SRC3) -o $(OBJ3)

$(OBJ4) : $(SRC4) $(HFILES)
	g++ $(INCDIR) -c $(CFLAGS) $(TCOMP) $(SHOW_ALLOCCOMP) $(SRC4) -o $(OBJ4)

$(ASM_PARSER).tab.o $(ASM_PARSER).yy.o :
	$(MAKE) -C parser all


debug: CFLAGS = -g -Wall -DDEBUG=1
debug: all

cleanup:
	@echo "Deleting object files..."
	rm -f src/*.o

clean:
	make -B
	make cleanup

clean-debug:
	make debug -B
	make cleanup
