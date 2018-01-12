#include<type_traits>

using namespace std;
template <class F, class R = typename result_of<F()>::type>
R call(F& f) {
    return f();
}
int answer() {
    return 42;
}

int main()
{
    call(answer);
    return 0;
}



#include<type_traits>
using namespace std;
template <class F>
auto call(F& f)->decltype(f()) {
    return f();
}

int f() {
    return 1;
}

int main() {

    call(f);
    return 0;
}


