#include "cec14_test_func.hxx"
#include "algorithm.hxx"
#include <vector>
#include <iostream>
#include <random>
#include <algorithm>
#include <cmath>

double *OShift, *M, *y, *z, *x_bound;
int ini_flag, n_flag, func_flag, *SS;

void freespace() {
  //free(x);
  //free(f);
  free(y);
  free(z);
  free(M);
  free(OShift);
  free(x_bound);
}

double test() {
  std::vector<double> x;
  for (int i = 0; i < 10; i++) {
    x.push_back(i + i/10.0);
  }

  int num_func;
  std::cout << "num_func > ";
  std::cin >> num_func;

  double f = 0;

  cec14_test_func(x.data(), &f, 10, 1, num_func);

  std::cout << f << std::endl;

  freespace();
  return f;
}

std::vector<bool> ANSBase::random_boolean(unsigned size, unsigned truthy) {
  std::vector<bool> values(size, false);
  std::vector<unsigned> unselected(size);
  // Fill unselected with increasing integers
  std::iota(unselected.begin(), unselected.end(), 0);

  for (unsigned i = 0; i < truthy && !unselected.empty(); ++i) {
    // Choose a previously unselected index
    std::uniform_int_distribution<int> distribution(0, unselected.size() - 1);
    int new_truthy = distribution(generator);
    values[unselected[new_truthy]] = true;
    unselected.erase(unselected.begin() + new_truthy);
  }

  return values;
}

solution ANSBase::random_solution() {
  solution blank;
  std::uniform_real_distribution<double> distribution(range_min, range_max);
  for (unsigned i = 0; i < dimensionality; ++i) {
    blank.push_back(distribution(generator));
  }

  return blank;
}

double ANSBase::value_of(solution sol) {
  std::map<solution, double>::iterator found = value.find(sol);

  if (found == value.end()) {
    double f;
    cec14_test_func(sol.data(), &f, dimensionality, 1, num_func);
    std::cerr << "\e[31mEvaluando: " << f << "\e[m" << std::endl;
    value[sol] = f;
    return f;
  } else {
    return (*found).second;
  }
}

solution ANSBase::run() {
  std::vector<solution> positions;
  // Randomly initialize the position of each individual
  std::generate_n(std::back_inserter(positions), population_size, [&]() { return random_solution(); });

  // Initialize the superior solution
  std::vector<solution> superiors(positions);

  // Record the best solution found by the whole population so far
  solution best = *std::max_element(superiors.begin(), superiors.end(), [&](const solution &a, const solution &b) { return value_of(a) < value_of(b); });

  std::normal_distribution<double> gaussian(0.0, gaussian_std);
  std::uniform_int_distribution<int> uniform(0, dimensionality - 2);

  unsigned max_generations = 1000; // this should be a parameter
  for (unsigned generation = 0; generation < max_generations; ++generation) {
    for (unsigned i = 0; i < population_size; ++i) {
      // Randomly selected 'ans_degree' dimensions for individual i
      std::vector<bool> selected = random_boolean(dimensionality, ans_degree);

      for (unsigned d = 0; d < dimensionality; ++d) {
        if (selected[d]) {
          unsigned g_d = uniform(generator);
          if (g_d >= i)
            g_d += 1;

          std::cerr << "\e[33m(g_d = " << g_d << ")\e[32mpositions[" << i << "][" << d << "] = (" << positions[i][d];
          positions[i][d] = superiors[g_d][d] + gaussian(generator) * std::fabs(superiors[g_d][d] - positions[i][d]);
          std::cerr << ")" << positions[i][d] << "\e[m" << std::endl;
        } else {
          std::cerr << "\e[32mpositions[" << i << "][" << d << "] = (" << positions[i][d];
          positions[i][d] = superiors[i][d] + gaussian(generator) * std::fabs(superiors[i][d] - positions[i][d]);
          std::cerr << ")" << positions[i][d] << "\e[m" << std::endl;
        }
      }

      if (value_of(positions[i]) < value_of(superiors[i])) // positions[i] better than superiors[i]
        superiors[i] = positions[i];

      if (value_of(positions[i]) < value_of(best)) { // positions[i] better than best
        best = positions[i];

        std::cerr << "better solution: {";
        for (auto& e : best)
          std::cerr << e << ", ";
        std::cerr << "}" << std::endl;
      }
    }
  }

  return best;
  return random_solution();
}

int main() {
  //test();
  ANSBase ans_instance(10, 10, 4, 10, 1, 0.5, -100, 100);
  solution found = ans_instance.run();

  std::cout << "{";
  for (auto& e : found)
    std::cout << e << ", ";
  std::cout << "}" << std::endl;
}
