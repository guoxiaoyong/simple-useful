#include <iostream>

#define E(L,I) (I < sizeof(L)) ? L[I] : 0


#define STR(X, L)                                                       \
  typename Expand<X,                                                    \
                  cstring<E(L,0),E(L,1),E(L,2),E(L,3),E(L,4), E(L,5),   \
                          E(L,6),E(L,7),E(L,8),E(L,9),E(L,10), E(L,11), \
                          E(L,12),E(L,13),E(L,14),E(L,15),E(L,16), E(L,17)> \
                  cstring<>, sizeof L-1>::type

#define CSTR(L) STR(cstring, L)

template<char ...C> struct cstring { };

template<template<char...> class P, typename S, typename R, int N>
struct Expand;

template<template<char...> class P, char S1, char ...S, char ...R, int N>
struct Expand<P, cstring<S1, S...>, cstring<R...>, N> :
  Expand<P, cstring<S...>, cstring<R..., S1>, N-1>{ };

template<template<char...> class P, char S1, char ...S, char ...R>
struct Expand<P, cstring<S1, S...>, cstring<R...>, 0> {
  typedef P<R...> type;
};

template<char ...S>
struct Test {
  static void print() {
    char x[] = { S... };
    std::cout << sizeof...(S) << std::endl;
    std::cout << x << std::endl;
  }
};

template<char ...C>
void process(cstring<C...>) {
  /* process C, possibly at compile time */
}

int main() {
  typedef STR(Test, "Hello folks") type;
  //type::print();

  //process(CSTR("Hi guys")());
  return 0;
}
