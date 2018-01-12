#include <vector>
#include <limits>
#include <iostream>

using namespace std;

template<typename T>
class Allocator {

public:
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

public:
    template<typename U>
    struct rebind {
        typedef Allocator<U> other;
    };

public:
    inline explicit Allocator() {
        cout << "create allocator!" << endl;
    }

    inline ~Allocator() {}
    inline explicit Allocator(Allocator const&) {}

    template<typename U>
    inline explicit Allocator(Allocator<U> const&) {}

    inline pointer address(reference r) {
        return &r;
    }

    inline const_pointer address(const_reference r) {
        return &r;
    }

    inline pointer allocate(size_type cnt,
                            typename std::allocator<void>::const_pointer = 0) {

        cout << "count = " << cnt << endl;
        return reinterpret_cast<pointer>(::operator new(cnt * sizeof (T)));
    }

    inline void deallocate(pointer p, size_type) {
        ::operator delete(p);
    }

    inline size_type max_size() const {
        return std::numeric_limits<size_type>::max() / sizeof(T);
    }

    inline void construct(pointer p, const T& t) {

        cout << "construct called" << endl;
        new(p) T(t);
    }

    inline void destroy(pointer p) {
        p->~T();
    }
};

/*
inline bool operator==(Allocator const&) {
    return true;
}

inline bool operator!=(Allocator const& a) {
    return !operator==(a);
}
*/

struct A {
    int a[10];

    A() {
        cout << "create A" << endl;
    }
};

int main(void) {

    std::vector<A, Allocator<A> > vec;

    for (int n = 0; n < 10; n++) {
        vec.push_back(A());
        cout << endl;
    }

    return 0;
}
