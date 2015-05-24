##
## SE3376.502 - Program 6
## Author: Justin Head (jxh122430@utdallas.edu)
##

## C++ compiler configuration
CXX = g++
CXXFLAGS = -Wall -I include  -Wno-write-strings
LIBS = -Llib -lcurses -lcdk -lboost_thread-mt -lboost_system

## General configuration
EXEC = program6
OBJS = program6.o

## Default target
all: clean $(EXEC)

## Main target
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBS) -o $@

## Clean target
clean:
	rm -f *.o
