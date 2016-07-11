#ifndef __ENHANCEMENTS
#define __ENHANCEMENTS
#include "algorithm.hxx"

class ANSMemory : public ANSBase {
  std::vector<bool> improved_memory;

public:
  ANSMemory(unsigned m, unsigned c, unsigned n, unsigned d, unsigned f, double sigma, double min, double max)
    :ANSBase(m, c, n, d, f, sigma, min, max), improved_memory(false, m) {}
  solution run();
};
#endif
