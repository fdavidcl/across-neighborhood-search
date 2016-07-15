#ifndef __ANSM
#define __ANSM
#include "algorithm.hxx"

class LANS : public ANSBase {
protected:
  unsigned initial_population_size;

public:
  LANS(unsigned m, unsigned c, unsigned n, unsigned d, unsigned f, double sigma, double min, double max)
    :ANSBase(m, c, n, d, f, sigma, min, max), improved_memory(false, m), initial_population_size(m) {}
  solution run();
};
#endif
