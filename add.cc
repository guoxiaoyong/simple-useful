#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {

    int a = stoi(argv[1]);
    char b = argv[2][0];
    int c = stoi(argv[3]);

    switch (b) {
    case '+':
        cout << (a+c) << endl;
        break;

    case '-':
        cout << (a-c) << endl;
        break;

    case '*':
        cout << (a*c) << endl;
        break;

    case '/':
        cout << (a/c) << endl;
        break;
    }
    return 0;
}
