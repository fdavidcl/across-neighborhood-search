#ifndef __ENHANCEMENTS
#define __ENHANCEMENTS
#include "algorithm.hxx"

class ANSMemory : public ANSBase {
protected:
  std::vector<bool> improved_memory;

public:
  ANSMemory(unsigned m, unsigned c, unsigned n, unsigned d, unsigned f, double sigma, double min, double max)
    :ANSBase(m, c, n, d, f, sigma, min, max), improved_memory(false, m) {}
  solution run();
};

#include "problemcec2014.h"
#include "problem.h"
//#include "solis.h"
#include "simplex.h"
//#include "cmaeshan.h"
#include "random.h"
#include "srandom.h"
//#include "domain.h"
#include "localsearch.h"

class HANS : public ANSBase {
protected:
  realea::ILocalSearch* ls;
  double optimize(solution& individual, unsigned evals);

  realea::ProblemCEC2014 cec2014;
  realea::ProblemPtr problem;

public:
  HANS(unsigned m, unsigned c, unsigned n, unsigned d, unsigned f, double sigma, double min, double max, realea::ILocalSearch* ls)
    :ANSBase(m, c, n, d, f, sigma, min, max), ls(ls),
    cec2014(d), problem(cec2014.get(f)) {}

  solution run();
};
#endif
