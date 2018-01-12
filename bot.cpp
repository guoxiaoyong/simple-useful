
#include <iostream>
#include <vector>

using namespace std;

int n = 0;

vector<int> F;
vector<int> E;
vector<int> C;


bool fun() {

    if (F.empty() && E.size() < 2 && C.size() < 4) return false;

    for (int k = 0; k < F.size(); k++) {

        F.pop_back();
        n++;
        E.push_back(1);
        C.push_back(1);
    }

    int ne = E.size()/2;
    for (int k = 0; k < ne; k++) {
        E.pop_back();
        E.pop_back();
        F.push_back(1);
    }

    int nc = C.size()/4;
    for (int k = 0; k < nc; k++) {
        C.pop_back();
        C.pop_back();
        C.pop_back();
        C.pop_back();
        F.push_back(1);
    }

    return true;
}

int main() {

    F.push_back(1);
    F.push_back(1);
    F.push_back(1);
    F.push_back(1);
    F.push_back(1);


    while(fun());

    cout << n << endl;
    cout << E.size() << endl;
    cout << C.size() << endl;


    return 0;
}
