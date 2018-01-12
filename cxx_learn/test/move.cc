#include <utility>

struct A {
    int d1;
    int d2;
};

A fun(A&& a) {

    A b;
    b.d1 = a.d1+11; 
    b.d2 = a.d2+13; 
    return b;
}

int main(void) {

    A a;
    a.d1 = 2;
    a.d2 = 3;


    A b = fun(std::move(a));

    return 0;
}

