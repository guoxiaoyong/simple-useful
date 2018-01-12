#include <iostream>
#include <string>

using namespace std;

string getName1() {

    return "tom";
}

const string& getName2() {

    static string name("mike");
    return name;
}

string& getName3() {

    static string name("jack");
    return name;
}



void printName(string&& name) {

    cout << "non-const rvalue reference:  " << name << endl;
}

void printName(const string&& name) {

    cout << "const rvalue reference:  " << name << endl;
}

void printName(string& name) {

    cout << "non-const regular reference:  " << name << endl;
}

void printName(const string& name) {

    cout << "const regular reference:  " << name << endl;
}

/*
void printName(string name) {

    cout << "pass by non-const value:  " << name << endl;
}
*/

void printName(const string name) {

    cout << "pass by const value:  " << name << endl;
}




int main(void) {

    //printName("bob");
    //printName(getName1());
    //printName(getName2());
    printName(getName3());
    return 0;
}

