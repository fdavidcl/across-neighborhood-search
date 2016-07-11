#include "enhancements.hxx"
#include <random>
#include <algorithm>
#include <cmath>
#include <functional>

solution ANSMemory::run() {
  std::vector<solution> positions;
  // Randomly initialize the position of each individual
  std::generate_n(std::back_inserter(positions), population_size, [&]() { return random_solution(); });

  // Initialize the superior solution
  std::vector<solution> superiors(positions);

  // Record the best solution found by the whole population so far
  solution best = *std::max_element(superiors.begin(), superiors.end(), [&](const solution &a, const solution &b) { return value_of(a) < value_of(b); });

  // These distributions will be later used for random number generation
  std::normal_distribution<double> gaussian(0.0, gaussian_std);
  std::uniform_int_distribution<int> uniform(0, population_size - 2);

  evaluations = 0;
  while (evaluations < max_evaluations && value_of(best) > 10e-8) {
    if (evaluations % 100 == 0)  // record diversity every 100 generations
      diversity.push_back(current_diversity(positions));

    for (unsigned i = 0; i < population_size; ++i) {
      std::vector<bool> selected;
      if (!improved_memory[i])
        // Randomly select 'ans_degree' dimensions for individual i
        selected = random_boolean(dimensionality, ans_degree);
      else
        selected = std::vector<bool>(false, dimensionality);

      for (unsigned d = 0; d < dimensionality; ++d) {
        if (selected[d]) {
          unsigned g_d = uniform(generator);
          if (g_d >= i)
            g_d += 1;

          positions[i][d] = superiors[g_d][d] + gaussian(generator) * std::fabs(superiors[g_d][d] - positions[i][d]);
        } else {
          positions[i][d] = superiors[i][d] + gaussian(generator) * std::fabs(superiors[i][d] - positions[i][d]);
        }
      }

      if (value_of(positions[i]) < value_of(superiors[i])) { // positions[i] better than superiors[i]
        superiors[i] = positions[i];
        improved_memory[i] = true;
      } else {
        improved_memory[i] = false;
      }

      if (value_of(positions[i]) < value_of(best)) { // positions[i] better than best
        best = positions[i];

        /*std::cerr << "better solution: {";
        for (auto& e : best)
          std::cerr << e << ", ";
        std::cerr << "} with value \e[33m" << (value_of(best) - optimum) << "\e[m" << std::endl;*/
      }
    }
  }

  return best;
}
