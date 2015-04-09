##
## SE3376.502 - Program 4
## Author: Justin Head (jxh122430@utdallas.edu)
##

## C++ compiler configuration
CXX = g++
CXXFLAGS = -Wall

## C compiler configuration (for Lex and YACC)
## The auto-generated code is usually C, but we've added C++ syntax so we'll need to compile it accordingly
CC = g++
CFLAGS = -x c++ -Wall

## Lex scanner configuration
LEX = flex
LFLAGS = -t

## YACC parser configuration
YACC = bison
YFLAGS = -dy

## General configuration
EXEC = program4
OBJS = parse.o scan.o program4.o
LOBJ = scan
YOBJ = parse

## Default target
all: clean $(EXEC)

## Main target
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)
	rm -f y.tab.h
	ln -sf $(EXEC) scanner
	ln -sf $(EXEC) parser

## Lex scanner target
$(LOBJ).o:
	$(LEX) $(LFLAGS) $(LOBJ).l > $(LOBJ).c
	$(CC) $(CFLAGS) -c -o $(LOBJ).o $(LOBJ).c
	rm -f $(LOBJ).c

## Clean target
clean:
	rm -f *.o $(EXEC) scanner parser

## Create tar.gz for project
tar:
	tar czvf $(EXEC)-jxh122430.tar.gz Makefile program4.cc parse.y scan.l input.txt output*.txt
