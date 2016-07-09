#include <vector>
#include <iostream>
#include "ans.cpp"

int main() {
  //test();

  const double RANGE_MIN = -100, RANGE_MAX = 100, GAUSSIAN_STD = 0.5;
  const std::vector<unsigned> dimensionality = {10, 30};

  for (int num_func = 1; num_func <= 30; ++num_func) {
    std::cerr << "Now optimizing function f" << num_func << std::endl;

    for (const auto& d : dimensionality) {
      std::cerr << "with dimensionality " << d << std::endl;
      ANSBase ans_instance(20, 20, 10, d, num_func, GAUSSIAN_STD, RANGE_MIN, RANGE_MAX);
      solution found = ans_instance.run();

      std::cout << "{";
      for (auto& e : found)
        std::cout << e << ", ";
      std::cout << "} with value \e[33m" << ans_instance.value_of(found) << "\e[m" << std::endl;
    }
  }
}
