#include <iostream>
#include <vector>

#include "any.hpp"

int main(void) {
  any t1(10);
  any t2(10.2);

  std::cout << any_cast<int>(t1) << std::endl;
  std::cout << t1.type().name() << std::endl;

  std::cout << any_cast<double>(t2) << std::endl;
  std::cout << t2.type().name() << std::endl;

  return 0;
}
