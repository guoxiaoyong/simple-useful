template <class T>
struct HasOrderSysID {
  typedef char yes;
  typedef int no;

  typedef char B[10];
  template <typename U, U u> struct HasDataField;
  template <typename C> static yes test(HasDataField<B C::*, &C::OrderSysID>*) {}
  template <typename> static no test(...) {}

  static const bool value = sizeof(test<T>(0)) == sizeof(yes);
};

struct A {
  char OrderSysID[10];
};

struct B {
  char OrderSysID[10];
};

struct C {
  int a;
  char OrderSysID[10];
};

#include <iostream>

int main(void) {
  std::cout << HasOrderSysID<A>::value << std::endl;
  std::cout << HasOrderSysID<B>::value << std::endl;
  std::cout << HasOrderSysID<C>::value << std::endl;
  std::cout << HasOrderSysID<A>::test<A>() << std::endl;
  return 0;
}
