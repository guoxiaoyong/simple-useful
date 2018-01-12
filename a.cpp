#include <iostream>
#include <memory>

using namespace std;

class A {

public:
    A() {
        cout << "A created!" << endl;
    }

    ~A() {
        cout << "A destroyed!" << endl;
    }
};


class delfun {

public:
    void operator() (A* p) {
        delete[] p;
    }
};

int main() {

/*
    std::unique_ptr<A[]> p1(new A[2]);
    std::shared_ptr<A> p2(new A[3], delfun());
*/

    char* ptr = new char[4096];
    A* a = new(ptr) A;
    A* b = new A;

    return 0;
}
