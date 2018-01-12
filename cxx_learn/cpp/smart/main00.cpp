#include "person.hpp"

template <typename T>
class smart_ptr {

public:
    smart_ptr(T* p):ptr(p) {}

    ~smart_ptr() {

        delete ptr;
    }

    T& operator*() {
        return *ptr;
    }

    T* operator->() {
        return ptr;
    }

private:
    T* ptr;
};

using namespace std;

int main() {

    smart_ptr<person> p(new person("Cici"));
    p->tell();

#if 0
    {
        smart_ptr<person> q = p;
        q -> tell();
    }
#endif
}


