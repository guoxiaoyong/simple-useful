#include <iostream>
#include <cstdio>
#include <cerrno>
#include <cstring>

struct Parent {
    int a;
    virtual void fun1() {
        std::cout << "Parent function 1" << std::endl;
        //std::cout << "a = " << a << std::endl;
    }
    virtual void fun2() {
        std::cout << "Parent function 2" << std::endl;
        //std::cout << "a = " << a << std::endl;
    }
};


struct Son: public Parent {
    int b;
    virtual void fun1() {
        std::cout << "Son function 1" << std::endl;
        std::cout << "a = " << a << std::endl;
        std::cout << "b = " << b << std::endl;
    }
    virtual void fun2() {
        std::cout << "Son function 2" << std::endl;
        std::cout << "a = " << a << std::endl;
        std::cout << "b = " << b << std::endl;
    }
};

//typedef void (Parent::*Fun)(void);

typedef void (*Fun)(void*);

int main(void) {
    Parent* Obj;
    Obj = new Son();
    Obj->a = 10;
    Obj->fun1();
    Obj->fun2();
    std::cout << Obj << std::endl;
    unsigned long* data = (unsigned long*)Obj;
    std::cout << (unsigned long*)data[0] << std::endl;
    Fun* p = (Fun*)data[0];
    p[0](Obj);
    p[1](Obj);
    std::cout << p[0] << std::endl;
    std::cout << p[1] << std::endl;
    std::cout << Obj._vptr << std::endl;
    return 0;
}
