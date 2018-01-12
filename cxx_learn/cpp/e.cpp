enum MyEnum { A, B, C };

class MyClass {

public:
    MyClass(double) {}
};

bool operator==(char, MyClass) {

    return true;
}

int main() {
    char aChar = 0;
    MyEnum anEnum = A;
    return aChar == anEnum; // What happens here? 
}
