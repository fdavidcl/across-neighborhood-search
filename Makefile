# Define source files and executable objectives
SOURCES=src/test.cpp src/ans.cpp include/cec14_test_func.hxx include/algorithm.hxx include/ans_memory.hxx src/ans_memory.cpp include/hybrid_ans.hxx src/hybrid_ans.cpp
EXE=bin/ans
CXXFLAGS= -std=c++17 -Wall -O3 -g
INCLUDES=-Iinclude/ -Ilib/localsearch/
LS=lib/localsearch/cmaeshan.cc lib/localsearch/random.cc lib/localsearch/domain.cc lib/localsearch/localsearch.cc lib/localsearch/simplex.cc lib/localsearch/origcmaes.cc lib/localsearch/solis.cc lib/localsearch/problem.cc lib/localsearch/srandom.cc lib/localsearch/problemcec2014.cc


# Default objective. Make will do the rest
default: $(EXE)

$(EXE): $(SOURCES) $(LS)
	$(CXX) $< $(LS) -o $@ $(INCLUDES) $(CXXFLAGS)

clean:
	rm -f $(EXE)

doc: doc/doc.pdf

%.pdf: %.md
	pandoc $< -o $@ --filter pandoc-citeproc

%.html: %.md
	pandoc $< -o $@ -t dzslides
