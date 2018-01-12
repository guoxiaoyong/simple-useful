//
//a person who can tell us his/her name.
//

#include<iostream>
#include<string>

using namespace std;

class person {

public:
    person(string);
    void tell();
    ~person();

private:
    string name;
};

void person::tell() {
    cout << "Hi! I am " << name << endl;
}

person::person(string name):name(name) {
}

person::~person() {
    cout << "Bye!" << endl;
}


