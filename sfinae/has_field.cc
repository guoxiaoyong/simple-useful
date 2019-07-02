#include <iostream>
#include <type_traits>

template <typename T>
struct HasOrderSysID {
  typedef char B[10];
  template<typename U, U u> struct HasField {};
  template<typename> struct Test : std::false_type {};
  template<> struct Test<HasField<B T::*, &T::OrderSysID>> : std::true_type {};
  static constexpr bool value = Test<T>::value;
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
  //std::cout << HasOrderSysID<C>::value << std::endl;
  //std::cout << HasOrderSysID<A>::test<A>() << std::endl;
  return 0;
}
