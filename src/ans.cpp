#include "cec14_test_func.hxx"
#include "algorithm.hxx"
#include <vector>
#include <iostream>
#include <random>
#include <algorithm>
#include <cmath>
#include <functional>

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
  chromosome blank;
  std::uniform_real_distribution<double> distribution(range_min, range_max);
  for (unsigned i = 0; i < dimensionality; ++i) {
    blank.push_back(distribution(generator));
  }

  return solution(blank, value_of(blank));
}

double ANSBase::value_of(chromosome& sol) {
  double f;
  cec14_test_func(sol.data(), &f, dimensionality, 1, num_func);
  evaluations += 1;
  return f - optimum;

  /*// Hay un problema en el manejo del map value. Da cores al volver de soluciones dadas por la búsqueda local :(
  std::map<solution, double>::iterator found = value.find(sol);

  if (found == value.end()) {
    if (evaluations >= max_evaluations) {
      return std::numeric_limits<double>::infinity();
    } else {
      double f;
      cec14_test_func(sol.data(), &f, dimensionality, 1, num_func);

      value[sol] = f - optimum;
      if (f - optimum < 10e-8)
        value[sol] = 0;

      evaluations += 1;
      return f - optimum;
    }
  } else {
    return (*found).second;
  }*/
}

double COAlgorithm::distance(const chromosome& a, const chromosome& b) {
  if (a.size() != b.size())
    return -1;

  chromosome difference;
  std::transform(a.begin(), a.end(), b.begin(),
                 std::back_inserter(difference), std::minus<double>());
  double squaresum = std::accumulate(difference.begin(), difference.end(), 0, [](const double& acc, const double& n) { return acc + n * n; });
  return std::sqrt(squaresum);
}

double ANSBase::current_diversity(std::vector<solution>& positions) {
  // Sum of all distances
  return std::accumulate(positions.begin(), positions.end(), 0, [&](const double& acc, const solution& a) {
    return std::accumulate(positions.begin(), positions.end(), acc, [&](const double& acc2, const solution& b) {
      return acc2 + distance(a.first, b.first);
    });
  });
}

solution& ANSBase::best_of(std::vector<solution>& individuals) {
  return *std::max_element(individuals.begin(), individuals.end(), [&](solution &a, solution &b) { return a.second < b.second; });
}

solution ANSBase::run() {
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

      if (positions[i].second < superiors[i].second) // positions[i] better than superiors[i]
        superiors[i] = positions[i];

      if (positions[i].second < best.second) { // positions[i] better than best
        best = positions[i];
      }
    }
  }

  return best;
}
