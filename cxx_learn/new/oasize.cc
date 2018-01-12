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

    A* a = new A[123];
    uint64_t* psz = (uint64_t*)((char*)a - 8);
    cout << *psz << endl;
    delete[] a;
    return 0;
}

