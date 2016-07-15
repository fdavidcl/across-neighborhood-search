#ifndef __ALGORITHM
#define __ALGORITHM
#include <vector>
#include <map>
#include <random>
#include <chrono>

typedef std::vector<double> chromosome;
typedef std::pair<chromosome, double> solution;

class COAlgorithm {
public:
  virtual solution run() = 0;
  double distance(const chromosome& a, const chromosome& b);
};

class ANSBase : public COAlgorithm {
protected:
  // Parameters and data necessary for the algorithm
  unsigned population_size,
    superior_num,
    ans_degree,
    dimensionality,
    max_evaluations,
    evaluations,
    generations,
    num_func;
  double gaussian_std, range_min, range_max, optimum;
  //std::map<solution, double> value;

  std::default_random_engine generator;

  // Other data recorded by the algorithm
  std::vector<double> diversity;

  solution random_solution();
  double current_diversity(std::vector<solution>& positions);
  std::vector<bool> random_boolean(unsigned size, unsigned truthy);
  solution& best_of(std::vector<solution>& individuals);

public:
  // population_size, superior_num, ans_degree, dimensionality, num_func, gaussian_std, range_min, range_max
  ANSBase(unsigned m, unsigned c, unsigned n, unsigned d, unsigned f, double sigma, double min, double max) :
    population_size(m),
    superior_num(c),
    ans_degree(n),
    dimensionality(d),
    num_func(f),
    gaussian_std(sigma),
    range_min(min),
    range_max(max),
    optimum(100 * f),
    max_evaluations(10000 * dimensionality),
    evaluations(0),
    generator(std::chrono::system_clock::now().time_since_epoch().count())
    {}

  double value_of(chromosome& sol);
  solution run();
  const std::vector<double>& get_diversity() const { return diversity; }
  unsigned get_generations() { return generations; }
  unsigned get_evaluations() { return evaluations; }
};
#endif
