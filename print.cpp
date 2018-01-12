#include <cstdio>
#include <iostream>
#include <stdexcept>

void Printf(const char *s) {
  while (*s) {
    if (*s == '%') {
      if (*(s + 1) == '%') {
        ++s;
      } else {
        throw std::runtime_error("invalid format string: missing arguments");
      }
    }
    std::cout << *s++;
  }
}

template <typename T, typename... Args>
void Printf(const char *s, T value, Args... args) {
  while (*s) {
    if (*s == '%') {
      if (*(s + 1) == '%') {
        ++s;
      } else {
        std::cout << value;
        // call even when *s == 0 to detect extra arguments
        Printf(s + 1, args...);
        return;
      }
    }
    std::cout << *s++;
  }
  throw std::logic_error("extra arguments provided to printf");
}

template <typename T1, typename T2> auto Max(const T1 &x1, const T2 &x2) {

  return std::max<T1>(x1, x2);
}

template <typename T1, typename T2, typename... Args>
auto Max(const T1 &x1, const T2 &x2, const Args &... args) {

  return Max(Max(x1, x2), args...);
}

int main(void) {
  printf("hello %% world %d\n", 100);
  Printf("hello %% world % %\n", 1001, 1.0 / 3.0);
  int x = Max(1, 2, 3);
  std::cout << x << std::endl;

  int y = Max(1, 2.0, 3);
  std::cout << y << std::endl;

  auto fun = Max<bool, int, double, char>;

  return 0;
}
