#include <iostream>
#include <string>

using namespace std;

class MyClass {

private:
    string name;

public:
    MyClass (string s) :name(s) {}
    ~MyClass() {
        cout << "Destroying " << name << endl;
    }
};

void fa();
void fb();
void fc();
void fd();

int main() {

    try {

        MyClass mainObj("M");
        fa();
        cout << "Mission accomplished!\n";

    } catch (const char *e) {

        cout << "exception: " << e << endl;
        cout << "Mission impossible!\n";
    }

    return 0;
}

void fa() {
    MyClass a("A");
    fb();
    cout << "return from fa()\n";
    return;
}

void fb() {
    MyClass b("B");
    fc();
    cout << "return from fb()\n";
    return;
}

void fc() {
    MyClass c("C");
    fd();
    cout << "return from fc()\n";
    return;
}

void fd() {
    MyClass d("D");
    throw "in fd(), something weird happened.";
    cout << "return from fd()\n";
    return;
}

