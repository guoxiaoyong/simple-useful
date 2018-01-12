#include <iostream>
#include <cstdlib>
#include <typeinfo>
#include <cassert>

struct default_deleter
{
    void operator () (void *p) { free(p); }
    operator bool() { return true;}
};

struct do_not_delete
{
    void operator () (void const *){}
    operator bool() { return true;}
};

template <typename T, typename D = default_deleter>
class unique_ptr
{
public:
    typedef T element_type;
    typedef D deleter_type;
    typedef T * pointer;

    explicit unique_ptr(pointer p = 0)
        : ptr_(p)
        , deleter_(default_deleter())
    {
    }

    unique_ptr(pointer p,deleter_type d)
        : ptr_(p)
        , deleter_(d)
    {
    }

    ~unique_ptr()
    {
        reset();
    }

    pointer get() const
    {
        return ptr_;
    }

    pointer release()
    {
        pointer tmp = ptr_;
        ptr_ = 0;
        return tmp;
    }

    void reset(pointer p = 0)
    {
        if (ptr_ != p){
            if (ptr_ )
                deleter_(ptr_);
            ptr_ = p;
        }
    }

    void swap(unique_ptr& u)
    {
        std::swap(ptr_,u.ptr_);
    }

    operator bool() const
    {
        return (ptr_ != 0);
    }
#if 0
    unique_ptr& operator=(unique_ptr<pointer,deleter_type> && other)
    {
        this->reset(other.release());
        deleter() = other.deleter();
        return *this;
    }
#endif
   private:
    unique_ptr(const unique_ptr&);
    unique_ptr& operator=(const unique_ptr&);

    pointer ptr_;
    deleter_type deleter_;
};

void testf(unique_ptr<int const,do_not_delete> ptr)
{
    //
}


int main()
{
    {
         unique_ptr<int> a((int *)malloc(sizeof(int)));
         unique_ptr<int,do_not_delete> c(a.get(),do_not_delete());
         unique_ptr<int> p(a.release());
    }
    {
        int const s = 10;
        testf(unique_ptr<int const,do_not_delete>(&s,do_not_delete()));
    }
    //unique_ptr<int,void(*)(void *)> a((int *)malloc(sizeof(int)),0);
}
