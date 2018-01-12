#include <vector>
#include <iostream>
#include <algorithm>
#include <functional>

using namespace std;


int main(void) {

    vector<int> a(20);
    iota(a.begin(), a.end(), 0);

    int x = 3;

    // fun and gun are different types,
    // use gdb ptype to see their types.
    auto gun = [x](int n) {
        return n%x == 0;
    };
    function<bool(int)> fun = gun;

    auto it = remove_if(a.begin(), a.end(), fun) ;
    for_each(a.begin(), it, [](decltype(*it)& n ) {
        cout << n << endl;
    });

    return 0;
}



