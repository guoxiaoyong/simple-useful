#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <iostream>

using namespace std;

union any_callable {
  void (*fun_prt)();
  void *fun_obj;
};

template <typename Func, typename R> struct fun_prt_manager {

  static R invoke(any_callable fp) {
    return reinterpret_cast<Func>(fp.fun_prt)();
  }

  static void destroy(any_callable fp) {}
};

template <typename Func, typename R> struct fun_obj_manager {

  static R invoke(any_callable fo) {
    return (*reinterpret_cast<Func *>(fo.fun_obj))();
  }

  static void destroy(any_callable fo) {
    delete reinterpret_cast<Func *>(fo.fun_obj);
  }
};

struct funtion_ptr_tag {};
struct funtion_obj_tag {};

template <typename Func> struct get_function_tag {
  typedef typename boost::mpl::if_<boost::is_pointer<Func>, funtion_ptr_tag,
                                   funtion_obj_tag>::type FunType;
};

template <typename Signature> class Function;

template <typename R> class Function<R()> {

  R (*invoke)(any_callable);
  void (*destory)(any_callable);
  any_callable fun;

public:
  ~Function() { clear(); }

  template <typename Func> Function &operator=(Func fun) {
    typedef typename get_function_tag<Func>::FunType fun_tag;
    assign(fun, fun_tag());
    return *this;
  }

  R operator()() const { return invoke(fun); }

  template <typename T> void assign(T funPtr, funtion_ptr_tag) {
    clear();
    invoke = &fun_prt_manager<T, R>::invoke;
    destory = &fun_prt_manager<T, R>::destroy;
    fun.fun_prt = reinterpret_cast<void (*)()>(funPtr);
  }

  template <typename T> void assign(T funObj, funtion_obj_tag) {
    clear();
    invoke = &fun_obj_manager<T, R>::invoke;
    destory = &fun_obj_manager<T, R>::destroy;
    fun.fun_obj = reinterpret_cast<void *>(new T(funObj));
  }

private:
  void clear() {
    if (destory) {
      destory(fun);
      destory = 0;
    }
  }
};

int TestFun() { return 0; }

class TestFunObj {
public:
  int p() { return 3; }
  int operator()() const { return 1; }
};

int main(int argc, char *argv[]) {
  Function<int()> fun;
  fun = &TestFun;
  cerr << fun() << endl;
  fun = TestFunObj();
  auto t = TestFunObj();
  cerr << t() << endl;
  cerr << TestFunObj()() << endl;
  cerr << fun() << endl;
  return 0;
}
