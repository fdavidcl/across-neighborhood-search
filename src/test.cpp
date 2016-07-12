#include <vector>
#include <iostream>
#include "ans.cpp"
#include "ans_memory.cpp"
#include "hybrid_ans.cpp"
#include "solis.h"
#include "simplex.h"
#include "localsearch.h"
#include "cmaeshan.h"

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

  // File management
  std::string results_filename = "out/results.log";
  if (argc > 1)
    results_filename = argv[1];
  std::string diversity_filename = "out/diversity.log";
  if (argc > 2)
    diversity_filename = argv[2];

  std::fstream results(results_filename, std::fstream::out);
  std::fstream div_file(diversity_filename, std::fstream::out);

  // Initialize a local search object
  realea::ILocalSearch* ls = new realea::SimplexDim();

  results << "10, 30," << std::endl;
  for (int num_func = 1; num_func <= 30; ++num_func) {
    std::cerr << "Now optimizing function f" << num_func << std::endl;

    for (const auto& d : dimensionality) {
      std::cerr << "with dimensionality " << d << std::endl;
      HANS ans_instance(20, 20, 10, d, num_func, GAUSSIAN_STD, RANGE_MIN, RANGE_MAX, ls);
      solution found = ans_instance.run();

      std::cerr << "{";
      for (auto& e : found.first) std::cerr << e << ", ";
      std::cerr << "} with value" << found.second << std::endl;
      results << std::scientific << found.second << ",";

      div_file << "f" << num_func << "d" << d << " ";
      for (const auto& datum : ans_instance.get_diversity()) {
        div_file << datum << " ";
      }
      div_file << std::endl;
    }

    results << std::endl;
  }
}
