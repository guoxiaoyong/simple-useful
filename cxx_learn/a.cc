#include <iostream>
#include <type_traits>
#include <typeinfo>
#include <cmath>

using namespace std;

template<typename T, typename T1>
typename std::result_of<T(T1)>::type 
call_func(T t, T1 t1) {

    return t(t1);
}


auto main(int argc, char* argv[]) -> int {

    typedef double (*Func)(double d);
    Func fun = sqrt;
    auto x = call_func(fun, 5.0);
    cout << typeid(x).name() << endl;

    return 0;
}

