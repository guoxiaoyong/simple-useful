#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <vector>

using namespace std;

struct Obj {

  int n;

  Obj(int m) { n = m; }

  void print() { cout << n << endl; }
};

namespace {

class placeholder {};

placeholder _1;
}

template <typename R, typename T, typename Arg> struct simple_bind_t {

  typedef R (T::*fn)(Arg);

  fn fn_;

  T t_;

  simple_bind_t(fn f, const T &t) : fn_(f), t_(t) {}

  R operator()(Arg &a) { return (t_.*fn_)(a); }
};

template <typename R, typename T, typename Arg>
simple_bind_t<R, T, Arg> simple_bind(R (T::*fn)(Arg), const T &t,
                                     const placeholder &) {
  return simple_bind_t<R, T, Arg>(fn, t);
}

template <typename R, typename T, typename Arg>
simple_bind_t<R, T, Arg> simple_bind(R (T::*fn)(Arg), const T &t) {
  return simple_bind_t<R, T, Arg>(fn, t);
}

class Test {

public:
  void do_stuff(const std::vector<int> &v) {

    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
  }
};

int main() {

  Test t;
  std::vector<int> vec(5, 12);
  vec.push_back(42);

  simple_bind(&Test::do_stuff, t, _1)(vec);
  simple_bind(&Test::do_stuff, t)(vec);

  std::vector<Obj> v;

  for (int n = 0; n < 10; n++)
    v.push_back(n);

  std::for_each(v.begin(), v.end(), std::mem_fn(&Obj::print));

  return 0;
}
