template<int num>
struct Box {
  static const char* n;
};

template<> const char* Box<1>::n = "hello";
template<> const char* Box<2>::n = "world";

#include <iostream>

int main() {
  Box<1> b;
  std::cout << b.n << std::endl;

  Box<2> bb;
  std::cout << bb.n << std::endl;
  std::cout << &bb.n << std::endl;
  return 0;
}
