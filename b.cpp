#include <iostream>

using namespace std;

void fun(int& n) {

    cout << "no const" << endl;
    cout << __func__ << " " << n << endl;
}

void fun(const int& n) {

    cout << "const" << endl;
    cout << __func__ << " " << n << endl;
}

int main() {

    int a = 10;
    const int b = 10;

    fun(10);
    fun(a);
    fun(b);
    return 0;
}

