#include "ans_memory.hxx"
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
  solution best = best_of(superiors);

  // These distributions will be later used for random number generation
  std::normal_distribution<double> gaussian(0.0, gaussian_std);
  std::uniform_int_distribution<int> uniform(0, population_size - 2);

  evaluations = 0;
  while (evaluations < max_evaluations && best.second > 10e-8) {
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

          positions[i].first[d] = superiors[g_d].first[d] + gaussian(generator) * std::fabs(superiors[g_d].first[d] - positions[i].first[d]);
        } else {
          positions[i].first[d] = superiors[i].first[d] + gaussian(generator) * std::fabs(superiors[i].first[d] - positions[i].first[d]);
        }
      }
      positions[i].second = value_of(positions[i].first);

      if (positions[i].second < superiors[i].second) { // positions[i] better than superiors[i]
        superiors[i] = positions[i];
        improved_memory[i] = true;
      } else {
        improved_memory[i] = false;
      }

      if (positions[i].second < best.second) { // positions[i] better than best
        best = positions[i];
      }
    }
  }

  return best;
}
