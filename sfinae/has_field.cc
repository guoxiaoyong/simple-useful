#include <iostream>
#include <type_traits>

template <class T>
struct HasOrderSysID {
  typedef char B[10];
  template <typename U, U u> struct HasDataField;
  template <typename C> struct Testr<HasDataField<B C::*, &C::OrderSysID>*) ; std::true_type {};
  template <typename> static std::false_type test(...) {}

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


int main(void) {
  std::cout << HasOrderSysID<A>::value << std::endl;
  std::cout << HasOrderSysID<B>::value << std::endl;
  std::cout << HasOrderSysID<C>::value << std::endl;
  std::cout << HasOrderSysID<A>::test<A>() << std::endl;
  return 0;
}
