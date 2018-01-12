#include <iostream>

using namespace std;

class iarray {

    int data[10];

public:
    iarray() {}
    int& operator[](int n);
    const int& operator[](int n) const;
};

int& iarray::operator[](int n) {
    cout << "non-const" << endl;
    return data[n];
}

const int& iarray::operator[](int n) const {
    cout << "const" << endl;
    return data[n];
}

int main(void) {

    const iarray ia;

    //ia[10] = 10;
    cout << ia[10] << endl;
    return 0;
}
