##
## SE3376.502 - Program 5
## Author: Justin Head (jxh122430@utdallas.edu)
##

## C++ compiler configuration
CXX = g++
CXXFLAGS = -Wall -I include
LIBS = -L lib -l rudeconfig

## General configuration
EXEC = cs3376dirmond
OBJS = program5.o parseCommandLine.o parseConfigFile.o inotifyFunctions.o loggingFunctions.o fileFunctions.o

## Default target
all: clean $(EXEC)

## Main target
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBS) -o $@

## Clean target
clean:
	rm -f *.o
