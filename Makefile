# Define source files and executable objectives
SOURCES=src/ans.cpp include/cec14_test_func.hxx include/algorithm.hxx
EXE=bin/ans
CXXFLAGS= -std=c++17 -Wall -O3
INCLUDES=include/

# Default objective. Make will do the rest
default: $(EXE)

$(EXE): $(SOURCES)
	$(CXX) $< -o $@ -I$(INCLUDES) $(CXXFLAGS)

clean:
	rm -f $(EXE)
