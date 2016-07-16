#include "l_hans.hxx"
#include "problemcec2014.h"
#include "problem.h"
//#include "solis.h"
//#include "simplex.h"
//#include "cmaeshan.h"
#include "random.h"
#include "srandom.h"
//#include "domain.h"
#include "localsearch.h"

solution LHANS::run() {
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
  generations = 0;
  while (evaluations < max_evaluations && best.second > 10e-8) {
    generations += 1;
    if (generations % 50 == 0)  // record diversity every 100 generations
      diversity.push_back(current_diversity(positions));

    for (unsigned i = 0; i < population_size; ++i) {
      // Randomly selected 'ans_degree' dimensions for individual i
      std::vector<bool> selected = random_boolean(dimensionality, ans_degree);

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

      if (positions[i].second < best.second) { // positions[i] better than best
        if (evaluations < max_local_ratio * max_evaluations)
          optimize(positions[i], 1000);

        best = positions[i];
      }

      if (positions[i].second < superiors[i].second) {// positions[i] better than superiors[i]
        superiors[i] = positions[i];
      }
    }
    
    unsigned next_population_size = std::round((2 - initial_population_size)/(double)max_evaluations * evaluations + initial_population_size);
    if (next_population_size < population_size) {
      std::vector<unsigned> indices(population_size);
      std::iota(indices.begin(), indices.end(), 0);
      std::sort(indices.begin(), indices.end(), [&](unsigned i, unsigned j) { return positions[i].second < positions[j].second; });

      // Reorder individuals and keep only the best `next_population_size` ones
      // This is as elegant as it gets (in C++)
      std::vector<solution> next_positions;
      std::transform(indices.begin(), indices.begin() + next_population_size, std::back_inserter(next_positions), [&](unsigned i) { return positions[i]; });

      std::vector<solution> next_superiors;
      std::transform(indices.begin(), indices.begin() + next_population_size, std::back_inserter(next_superiors), [&](unsigned i) { return superiors[i]; });

      // Replace current data with data for next generation
      population_size = next_population_size;
      positions = next_positions;
      superiors = next_superiors;
    }
  }

  std::cerr << "LS used " << evaluations_ls << " evals while ANS used " << evaluations - evaluations_ls << std::endl;

  return best;
}
