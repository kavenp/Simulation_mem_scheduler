
# Makefile for Computer Systems Project 1
# Author: Kaven Peng kavenp 696573

## CC  = Compiler.
## CFLAGS = Compiler flags.
CC	= gcc
CFLAGS 	= -Wall


## OBJ = Object files.
## SRC = Source files.
## EXE = Executable name.

SRC =		main.c queue.c memory.c simulation.c
OBJ =		main.o queue.o memory.o simulation.o
EXE = 		simulation

## Top level target is executable.
$(EXE):	$(OBJ)
		$(CC) $(CFLAGS) -o $(EXE) $(OBJ) -lm


## Clean: Remove object files and core dump files.
clean:
		/bin/rm $(OBJ) 

## Clobber: Performs Clean and removes executable file.

clobber: clean
		/bin/rm $(EXE) 

## Dependencies

main.o:	process.h qnode.h qfuncs.h memnode.h memfuncs.h simfuncs.h
queue.o:	process.h qnode.h qfuncs.h
memory.o:   process.h memnode.h memfuncs.h
simulation.o: process.h qnode.h qfuncs.h memnode.h memfuncs.h simfuncs.h