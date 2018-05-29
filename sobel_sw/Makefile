# Source, Executable, Includes, Library Defines

INCL   = -I/eda/bin/cwb/cyber/include
SRC    = sobel.c
OBJ    = $(SRC:.c=.o)
EXE=   sobel.exe

# Compiler, Linker Defines
CC = gcc
CFLAGS= -pedantic -Wall -O3 -DC
#LIBPATH = -L.
LDFLAGS = -o $(EXE) $(LIBPATH) $(LIBS)
CFDEBUG = -DC  -Wall -g -DDEBUG $(LDFLAGS)
RM      = /bin/rm -f

default: all

# Compile and Assemble C Source Files into Object Files
$(OBJ):  
	$(CC) -c -Wall $(CFLAGS) $(INCL)  $*.c


$(OBJHD):
	$(CC) -c -Wall -DRESHD  $(CFLAGS) $(INCL)  $*.c


# Link all Object Files with external Libraries into Binaries
all:	$(OBJ)
	$(CC) $(LDFLAGS) $(OBJ)


RESHD:
	 $(CC) -c -Wall -DRESHD -g  $(CFLAGS) $(INCL)  $(SRC)
	$(CC) $(LDFLAGS) $(OBJ)

# Create a gdb/dbx Capable Executable with DEBUG flags turned on
debug:
	 $(CC) $(CFDEBUG) $(SRC)


# Clean Up Objects, Exectuables, Dumps out of source directory
clean:
	 $(RM) $(OBJ) $(EXE) 


