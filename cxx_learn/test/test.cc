struct A {

    virtual int fun(int);
    virtual int gun(int);
    virtual int hun(int);
};

int A::fun(int a) {

    return a+1;
}


int A::gun(int a) {

    return a+10;
}


int A::hun(int a) {

    return a+100;
}

//#include <cstdio>

int main(void) {

    int (A::*p1)(int);
    int (A::*p2)(int);
    int (A::*p3)(int);

    p1 = &A::fun;
    p2 = &A::gun;
    p3 = &A::hun;

    A a;
    A* b = &a;

#define NITERS 10000000000UL
    unsigned long n;

    //for (n = 0; n < NITERS; n++) a.fun(22);
    for (n = 0; n < NITERS; n++) b->gun(23);
    //for (n = 0; n < NITERS; n++) (a.*p3)(24);

    //printf("%p\n", p3);

    return 0;
}


