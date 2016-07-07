#include <vector>
#include <map>
#include <random>

typedef std::vector<double> solution;

class COAlgorithm {
public:
  virtual solution run() = 0;
};

class ANSBase : public COAlgorithm {
  unsigned population_size,
    superior_num,
    ans_degree,
    dimensionality,
    num_func;
  double gaussian_std, range_min, range_max;
  std::map<solution, double> value;

  std::default_random_engine generator;

  double value_of(solution sol);

  solution random_solution();
  std::vector<bool> random_boolean(unsigned size, unsigned truthy);

public:
  ANSBase(unsigned m, unsigned c, unsigned n, unsigned d, unsigned f, double sigma, double min, double max)
    :population_size(m), superior_num(c), ans_degree(n), dimensionality(d), num_func(f),
     gaussian_std(sigma), range_min(min), range_max(max) {}

  solution run();
};
