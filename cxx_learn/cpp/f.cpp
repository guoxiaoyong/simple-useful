#include <memory>
#include <set>
#include <map>

// dual index map
template<typename K1, typename K2, typename T>
class dimap {

    typedef

private:
    typedef std::shared_ptr<T> P;
    std::set<P> table;
    std::map<K1,P> index1;
    std::map<K2,P> index2;

public:

    T& operator[](const K1& idx) {
    }

    const T& operator[](const K1& idx) const {
    }

    T& operator[](const K2& idx) {
    }

    const T& operator[](const K2& idx) const {
    }

    void erase(const K1& idx) {
    }

    void erase(const K2& idx) {
    }

    bool find(const K1& idx) {
    }

    bool find(const K2& idx) {
    }



};

