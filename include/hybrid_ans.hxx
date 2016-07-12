#ifndef __HANS
#define __HANS
#include "problemcec2014.h"
#include "problem.h"
#include "localsearch.h"

class HANS : public ANSBase {
protected:
  realea::ILocalSearch* ls;
  double optimize(solution& individual, unsigned evals);

  realea::ProblemCEC2014 cec2014;
  realea::ProblemPtr problem;
  unsigned evaluations_ls;

public:
  HANS(unsigned m, unsigned c, unsigned n, unsigned d, unsigned f, double sigma, double min, double max, realea::ILocalSearch* ls)
    :ANSBase(m, c, n, d, f, sigma, min, max), ls(ls), evaluations_ls(0),
    cec2014(d), problem(cec2014.get(f)) {}

  solution run();
};
#endif
