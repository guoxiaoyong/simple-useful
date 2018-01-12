#include <iostream>

using namespace std;

struct A {

    A() {
        cout << "create" << endl;
    }

    A(const A& a) {
        cout << "copy constructor" << endl;
    }

    A& operator=(const A&a) {

        cout << "assignment constructor" << endl;
    }

    ~A() {
        cout << "destroy" << endl;
    }
};

A fun() {

    A a = A();
    return a;
}

int main(void) {

    A& a = fun();

    cout << "end" << endl;
    return 0;
}
