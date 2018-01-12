#include <string>
#include <iostream>

template<const char* name>
struct Helper {
  static const char* mod_name;
};

template<char... args>
struct Root {
  const char str[sizeof... (args)] = {args...};
  static const char* num;
};

template<char... args>
const char* Root<args...>::num = "good";

template<>
const char* Root<'a'>::num = "hello";

int main() {
  Root<'a', 'b', 'z'> r;
  std::cout << r.str << std::endl;
  std::cout << r.num << std::endl;

  Root<'a'> rr;
  std::cout << rr.str << std::endl;
  std::cout << rr.num << std::endl;

  return 0;
}
