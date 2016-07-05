#include "cec14_test_func.h"
#include "algorithm.h"
#include <vector>
#include <iostream>

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


int main() {
  test();
}
