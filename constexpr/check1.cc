// N: string length, P: current position
// count: number of consecutive `_' that has been observed.
template <int N, int P> struct Counter {
  static_assert(N < 129,
                "The length of Module's full pathname must be less than 129!");
  static_assert(P < N,
                "Position must be less than the length of the input string!");
  constexpr static int count(int count, const char (&str)[N]) {
    return count == 3
               ? 3
               : (Counter<N, P + 1>::count(str[P] == '_' ? count + 1 : 0, str));
  }
};

template <int N> struct Counter<N, N> {
  constexpr static int count(int count, const char (&str)[N]) {
    return count;
  }
};

template <int N> constexpr static bool IsValidName(const char (&modname)[N]) {
  /*
    static_assert(
        modname[0] == 'c' && modname[1] == 'c' && modname[2] == '.',
        "module path must be starting with `cc.`");
        */
  return Counter<N, 0>::count(0, modname) != 3;
}

#include <iostream>
int main() {
  constexpr bool ret1 = IsValidName("a.b.c");
  constexpr bool ret2 = IsValidName("a.___b.c");
  return 0;
}
