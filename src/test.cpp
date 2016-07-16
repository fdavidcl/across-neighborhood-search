#include <vector>
#include <iostream>
#include "ans.cpp"
#include "ans_memory.cpp"
#include "hybrid_ans.cpp"
#include "l_ans.cpp"
#include "l_hans.cpp"
#include "solis.h"
#include "simplex.h"
#include "localsearch.h"
#include "cmaeshan.h"
#include <sstream>

//These are already declared in lib/localsearch/problemcec2014.cc
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

/*
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
}*/

int main(int argc, char* argv[]) {
  const double RANGE_MIN = -100, RANGE_MAX = 100, GAUSSIAN_STD = 0.5;
  const std::vector<unsigned> dimensionality = {10, 30};

  std::string variant = "ans_basic";
  if (argc < 2) {
    std::cerr << "No variant of ANS specified, assuming basic ANS." << std::endl;
  } else {
    variant = argv[1];
  }

  assert(variant == "ans_basic" || variant == "ans_memory" || variant == "hans_simplex" ||
    variant == "hans_cmaes" || variant == "hans_sw" || variant == "l_ans" ||
    variant == "l_hans_cmaes" || variant == "l_hans_sw" || variant == "l_hans_simplex");

  // File management
  std::string results_filename = "out/results.log";
  if (argc > 2)
    results_filename = argv[2];
  std::string diversity_filename = "out/diversity.log";
  if (argc > 3)
    diversity_filename = argv[3];

  std::fstream results(results_filename, std::fstream::out);
  std::fstream div_file(diversity_filename, std::fstream::out);

  // Initialize a local search object
  realea::ILocalSearch* ls;

  if (variant == "hans_simplex" || variant == "l_hans_simplex") {
    ls = new realea::SimplexDim();
  } else if (variant == "hans_cmaes" || variant == "l_hans_cmaes") {
    ls = new realea::CMAESHansen("cmaesinit.par");
    ((realea::CMAESHansen*)ls)->searchRange(0.1);
  } else if (variant == "hans_sw" || variant == "l_hans_sw") {
    ls = new realea::SolisWets();
    ((realea::SolisWets*)ls)->setDelta(0.2);
  }

  unsigned population_size = 20;
  if (argc > 4) {
    std::stringstream ss;
    ss << argv[4];
    ss >> population_size;
  }

  results << "10, 30," << std::endl;
  for (int num_func = 1; num_func <= 30; ++num_func) {
    std::cerr << "Now optimizing function f" << num_func << std::endl;

    for (const auto& d : dimensionality) {
      std::cerr << "with dimensionality " << d << std::endl;

      ANSBase* ans_instance;
      if (variant == "ans_basic")
        ans_instance = new ANSBase(population_size, population_size, 10, d, num_func, GAUSSIAN_STD, RANGE_MIN, RANGE_MAX);
      else if (variant == "ans_memory")
        ans_instance = new ANSMemory(population_size, population_size, 10, d, num_func, GAUSSIAN_STD, RANGE_MIN, RANGE_MAX);
      else if (variant == "l_ans")
        ans_instance = new LANS(population_size, population_size, 10, d, num_func, GAUSSIAN_STD, RANGE_MIN, RANGE_MAX);
      else if (variant == "hans_simplex" || variant == "hans_cmaes" || variant == "hans_sw" )
        ans_instance = new HANS(population_size, population_size, 10, d, num_func, GAUSSIAN_STD, RANGE_MIN, RANGE_MAX, ls, 0.4);
      else
        ans_instance = new LHANS(100, 100, 10, d, num_func, GAUSSIAN_STD, RANGE_MIN, RANGE_MAX, ls, 0.4);

      solution found = ans_instance->run();

      std::cerr << "{";
      for (auto& e : found.first) std::cerr << e << ", ";
      std::cerr << "} with value " << found.second << "(" <<
        ans_instance->get_evaluations() << " evaluations - " <<
        ans_instance->get_generations() << " generations)" << std::endl;
      results << std::scientific << found.second << ",";

      div_file << "f" << num_func << "d" << d << " ";
      for (const auto& datum : ans_instance->get_diversity()) {
        div_file << datum << " ";
      }
      div_file << std::endl;

      delete ans_instance;
    }

    results << std::endl;
  }
}
