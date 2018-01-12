#include "callback.h"
#include <iostream>

int add(int a, int b) { return a + b; };

int main(void) {
  auto fun = NewCallback(&add);
  std::cout << "fun address: " << fun << std::endl;
  std::cout << fun->Run(12, 9) << std::endl;

  auto gun = NewCallback(&add, 11);
  std::cout << gun->Run(3) << std::endl;

  auto foo = NewPermanentCallback(&add, 9);
  std::cout << foo->Run(91) << std::endl;
  std::cout << foo->Run(191) << std::endl;

  return 0;
}
