/*
template argument involves template parameter(s) A partially specialized non -
    type argument expression shall not involve a
    template parameter of the partial specialization except when the argument
    expression is a simple identifier.
    */

template <typename T>
struct DefOpt {
  static const int value = 0;
};

template <typename T>
struct DefClass {
  typedef T DefT;
};

template <
    typename T,
    typename V = typename DefClass<T>::DefT,
    int val = DefOpt<T>::value>
struct DefAccPol {};

template <typename T>
struct A {};

template <>
struct DefAccPol<int> {};

template <typename T>
struct DefAccPol<A<T>> {};

int main() {
  DefAccPol<A<int>> at;
}

/*
===compiler output===
error: template argument ‘DefOpt<A<T> >::value’ involves template parameter(s)

the reason int val=DefOpt<T>::value


the type (DefClass<T>::DefT) is fine.

template<>
struct DefAccPol< int >{
};
is fine as well, because it has no templates
only the combination of int val=.. and template<typename T> partial
specialization is bad

similarly


template<typename T>
struct DefAccPol< A<T>,T,DefOpt<T>::value >{
};


is not accepted, but


template<typename T>
struct DefAccPol< A<T>,T,0 >{
};

is ok
*/
