#include <exception>

template<typename T,
         template<typename Elem, typename Allocator = std::allocator<Elem>> class Container = std::vector>
struct Stack
{
    void push(const T& elem)
    {
        elems.push_back(elem);
    }

    T pop()
    {
        //if(empty()) throw std::out_of_range("Stack<>::pop: empty!");

        auto elem = elems.back();
        elems.pop_back();
        return elem;
    }

    bool empty() const
    {
        return elems.empty();
    }

private:
    Container<T> elems;
};
