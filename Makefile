# Define source files and executable objectives
SOURCES=src/ann.cpp
EXE=bin/ann
CXXFLAGS= -std=c++11 -Wall

# Default objective. Make will do the rest
default: $(EXE)

bin/ann: src/ann.cpp
	g++ $< -o $@
