# Define source files and executable objectives
SOURCES=src/ann.cpp include/cec14_test_func.hxx
EXE=bin/ann
CXXFLAGS= -std=c++11 -Wall
INCLUDES=include/

# Default objective. Make will do the rest
default: $(EXE)

bin/ann: $(SOURCES)
	$(CXX) $< -o $@ -I$(INCLUDES)
