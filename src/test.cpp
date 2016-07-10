#include <vector>
#include <iostream>
#include "ans.cpp"

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

int main() {
  const double RANGE_MIN = -100, RANGE_MAX = 100, GAUSSIAN_STD = 0.5;
  const std::vector<unsigned> dimensionality = {10, 30};
  //std::map<unsigned, std::vector<std::pair<solution, double>>> results;

  std::fstream results("out/results.log", std::fstream::out);

  results << "10, 30," << std::endl;
  std::cerr << "\e[32m";
  for (int num_func = 1; num_func <= 30; ++num_func) {
    std::cerr << "Now optimizing function f" << num_func << std::endl;

    for (const auto& d : dimensionality) {
      std::cerr << "with dimensionality " << d << std::endl;
      ANSBase ans_instance(20, 20, 10, d, num_func, GAUSSIAN_STD, RANGE_MIN, RANGE_MAX);
      solution found = ans_instance.run();
      //results[d].push_back({found, ans_instance.value_of(found)});

      std::cerr << "{";
      for (auto& e : found)
        std::cerr << e << ", ";
      std::cerr << "} with value" << ans_instance.value_of(found) << std::endl;
      results << ans_instance.value_of(found) << ",";
    }

    results << std::endl;
  }
  std::cerr << "\e[m";

  /*
  for (auto& pair : results) {
    std::cout << pair.first << " dimensions:" << std::endl;

    for (auto& sol : pair.second) {
      std::cout << sol.second << ", ";
    }

    std::
  }*/
}
