#include <iostream>

using namespace std;

/*
int fun(int a) {
}
*/

int fun(const int a) {

}

class A {
public:

    A() {
        cout << "A created" << endl;
        a= 10;
    }
    int a;
};

class AA {
public:

    AA() {
        cout << "AA created" << endl;
        a = 111;
    }

    AA& operator=(const AA& x) {
        cout << "assignment" << endl;
        a = x.a;
        return *this;
    }

    AA& operator=(AA& x) {
        cout << "non-const assignment" << endl;
        a = x.a;
        return *this;
    }



    AA(const AA& x) {
        cout << "copy constructor" << endl;
        a = x.a;
    }



    int a;
};

class B: public A, public AA {
public:

    int a;
};

void fun(AA a) {

    cout << "value = " << a.a << endl;
    cout << "AA" << endl;
}


/*
void fun(A) {

    cout << "A" << endl;
}
*/




/*
void fun(B) {

    cout << "B" << endl;
}
*/

void xfun(int const * const x){}

int main(void) {

    B b;
    //fun(a);
    fun(b);

    AA xx;
    const AA yy;
    xx = yy;

    static_cast<AA>(b);
    //cout << static_cast<AA>(b).a << endl;
    return 0;
}


