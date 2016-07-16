#ifndef __LHANS
#define __LHANS
#include "algorithm.hxx"

class LHANS : public HANS {
protected:
  unsigned initial_population_size;

public:
  LHANS(unsigned m, unsigned c, unsigned n, unsigned d, unsigned f, double sigma, double min, double max, realea::ILocalSearch* ls, double mlr)
    :HANS(m, c, n, d, f, sigma, min, max, ls, mlr), initial_population_size(m) {}
  solution run();
};
#endif
