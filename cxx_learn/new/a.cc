struct A {

    int data[100];

    A() {

        data[0]= 123;
    }

    ~A() {
        data[0] = 321;
    }
};

#include <iostream>
using namespace std;

int main(void) {

    uint64_t p[2048];

    A* a = new((void*)(p)) A[2];
    cout << a << endl;
    cout << p << endl;

    return 0;
}

