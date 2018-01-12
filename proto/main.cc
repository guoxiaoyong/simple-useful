#include <iostream>
#include "person.pb.h"


int main(void) {
  Person person;
  std::cout << person.has_name() << "\n";
  auto str = new std::string("hello", 64);
  person.set_allocated_name(str);
  std::cout << person.has_name() << "\n";

  person.clear_name();
  std::cout << str->c_str() << std::endl;
  std::cout << str->capacity() << std::endl;
  std::cout << person.has_name() << "\n";

  person.mutable_name();
  std::cout << person.has_name() << "\n";
  return 0;
}
