struct A {

    int data[100];

    A() {

       data[0]= 123;
    }

    ~A() {
       data[0] = 321;
    }
};

#include <new>
#include <iostream>

int main(int argc, char* argv[]) {

    char m[1024];

    A* p = new A[13];

    asm("nop");
    A* p1 = new A();

    asm("nop");
    A* p2 = new(std::nothrow) A();

    asm("nop");
    A* p3 = new((void*)m) A();

    asm("nop");
    delete p1;

    asm("nop");
    delete p2;

    asm("nop");
    p3->~A();

    return 0;
}
