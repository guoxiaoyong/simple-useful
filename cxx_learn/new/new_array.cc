struct A {

    int data[100];

    A() {

       data[0]= 123;
    }

    ~A() {
       data[0] = 321;
    }
};

using namespace std;

int main(void) {

    asm("nop");
    int* pint = new int[100];

    asm("nop");
    delete[] pint;

    asm("nop");
    A* a = new A[123];

    asm("nop");
    delete[] a;

    return 0;
}

