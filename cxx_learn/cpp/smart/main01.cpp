#include "person.hpp"

template <typename T>
class smart_ptr {

public:
    smart_ptr():ptr(0),ref_cnt(0) {
        ref_cnt = new int(0);
        (*ref_cnt)++;
    }

    smart_ptr(T* p):ptr(p) {
        ref_cnt = new int(0);
        (*ref_cnt)++;
    }


    ~smart_ptr() {
        if(--(*ref_cnt) == 0) {
            delete ref_cnt;
            delete ptr;
        }
    }

    T& operator*() {
        return *ptr;
    }

    T* operator->() {
        return ptr;
    }

    smart_ptr(const smart_ptr<T>& sp):ptr(sp.ptr),ref_cnt(sp.ref_cnt) {
        (*ref_cnt)++;
    }

    smart_ptr<T>& operator=(const smart_ptr<T>& sp) {

        if(&sp != this) {
            if(--(*ref_cnt) == 0) {
                delete ref_cnt;
                delete ptr;
            }
            ptr = sp.ptr;
            ref_cnt = sp. ref_cnt;
            (*ref_cnt)++;
        }
        return *this;
    }

private:

    T* ptr;
    int* ref_cnt;
};


using namespace std;

int main() {

    smart_ptr<person> r;
    smart_ptr<person> p(new person("Cici"));
    p -> tell();
    {
        smart_ptr<person> q = p;
        q -> tell();
        r = q;
        smart_ptr<person> s(r);
        s -> tell();
    }
    r -> tell();
}

