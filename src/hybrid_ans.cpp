#include "hybrid_ans.hxx"
#include "problemcec2014.h"
#include "problem.h"
//#include "solis.h"
//#include "simplex.h"
//#include "cmaeshan.h"
#include "random.h"
#include "srandom.h"
//#include "domain.h"
#include "localsearch.h"

double HANS::optimize(solution& individual, unsigned evals) {
  //solution copia(individual);

  /* Some code taken from lib/localsearch/example.cc - GPLv3 */

  // Get the function fun for dimension dim
  //realea::ProblemCEC2014 cec2014(dimensionality);
  //realea::ProblemPtr problem = cec2014.get(num_func);
  // Domain is useful for clipping solutions
  //realea::DomainRealPtr domain = problem->getDomain();

  //realea::SimplexDim ls;
  realea::ILSParameters *ls_options;

  // Init the random with the seed
  int seed = time(NULL);
  Random random(new SRandom(seed));

  // The following options are common for all LS methods
  // Set the problem to allow the LS to eval solutions
  ls->setProblem(problem.get());
  // Set the random to generate mutations
  ls->setRandom(&random);
  // Get the initial parameters
  ls_options = ls->getInitOptions(individual.first);

  //std::cerr << "Antes {"; for (auto& t:individual.first) std::cerr << t << " "; std::cerr << "}\n";
  unsigned real_evals = ls->apply(ls_options, individual.first, individual.second, evals);
  //std::cerr << "Después {"; for (auto& t:copia.first) std::cerr << t << " "; std::cerr << "}\n";
  evaluations += real_evals;
  evaluations_ls += real_evals;

  return individual.second;
}

solution HANS::run() {
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
  }

  std::cerr << "LS used " << evaluations_ls << " evals while ANS used " << evaluations - evaluations_ls << std::endl;

  return best;
}
