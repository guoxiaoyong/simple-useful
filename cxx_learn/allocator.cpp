// xmemory internal header (from <memory>)
#pragma once
#ifndef _XMEMORY_
#define _XMEMORY_
#ifndef RC_INVOKED
#include <cstdlib>
#include <new>
#include <xutility>

// 说明：std::allocator将内存分配与释放、对象的构造与销毁独立出来，
// 分别为内存分配、对象构造、对象销毁、内存释放。

#pragma pack(push,_CRT_PACKING)
#pragma warning(push,3)

// 由于下方使用了operator new函数，因此需要先取消new的宏定义
#pragma push_macro("new")
#undef new

#pragma warning(disable: 4100)

// 这里是内存分配
// TEMPLATE FUNCTION _Allocate
template<class T> inline
T* _Allocate(size_t _Count, T*) 
{   // allocate storage for _Count elements of type _Ty
    void *_Ptr = 0;

    if (_Count <= 0)
        _Count = 0;
    else if (((size_t)(-1) / sizeof (_Ty) < _Count)
             || (_Ptr = ::operator new(_Count * sizeof (_Ty))) == 0)
        _THROW_NCEE(bad_alloc, 0);

    return ((_Ty  *)_Ptr);
}

// 使用 placement new 进行对象的拷贝构造放置
// TEMPLATE FUNCTION _Construct
template<class _Ty1,
         class _Ty2> inline
void _Construct(_Ty1  *_Ptr, _Ty2&& _Val)
{   // construct object at _Ptr with value _Val
    void  *_Vptr = _Ptr;
    ::new (_Vptr) _Ty1(_STD forward<_Ty2>(_Val));
}

// 使用 placement new 进行对象的默认构造放置
template<class _Ty1> inline
void _Construct(_Ty1  *_Ptr)
{   // construct object at _Ptr with default value
    void  *_Vptr = _Ptr;

    ::new (_Vptr) _Ty1();
}

// 对象销毁（析构函数）
// TEMPLATE FUNCTION _Destroy
template<class _Ty> inline
void _Destroy(_Ty  *_Ptr)
{   // destroy object at _Ptr
    _Ptr->~_Ty();
}

// 内嵌字符串类型不用析构，特化版本供优化使用
template<> inline
void _Destroy(char  *)
{   // destroy a char (do nothing)
}

template<> inline
void _Destroy(wchar_t  *)
{   // destroy a wchar_t (do nothing)
}

#ifdef _NATIVE_WCHAR_T_DEFINED
template<> inline
void _Destroy(unsigned short  *)
{   // destroy a unsigned short (do nothing)
}
#endif /* _NATIVE_WCHAR_T_DEFINED */

// 根据指针类型选择是否为 POD 数据类型
// TEMPLATE FUNCTION _Destroy_range
template<class _Alloc> inline
void _Destroy_range(typename _Alloc::pointer _First,
                    typename _Alloc::pointer _Last, _Alloc& _Al)
{   // destroy [_First, _Last)
    _Destroy_range(_First, _Last, _Al, _Ptr_cat(_First, _Last));
}

// 非 POD 数据类型需要显示调用析构函数
template<class _Alloc> inline
void _Destroy_range(typename _Alloc::pointer _First,
                    typename _Alloc::pointer _Last, _Alloc& _Al,
                    _Nonscalar_ptr_iterator_tag)
{   // destroy [_First, _Last), arbitrary type
    for (; _First != _Last; ++_First)
        _Dest_val(_Al, _First);
}

// POD 数据类型不需要调用析构函数
template<class _Alloc> inline
void _Destroy_range(typename _Alloc::pointer _First,
                    typename _Alloc::pointer _Last, _Alloc& _Al,
                    _Scalar_ptr_iterator_tag)
{   // destroy [_First, _Last), scalar type (do nothing)
}

template<class T> inline
T* addressof(T& Val) {   
    return ((T*)&(char&)Val);
}

template<class T>
struct _Allocator_base
{   
    typedef T value_type;
};

template<class T>
struct _Allocator_base<const T>
{   
    typedef T value_type;
};

// TEMPLATE CLASS allocator
template<class _Ty>
class allocator
    : public _Allocator_base<_Ty>
{   // generic allocator for objects of class _Ty
public:
    typedef _Allocator_base<_Ty> _Mybase;
    typedef typename _Mybase::value_type value_type;

    typedef value_type  *pointer;
    typedef value_type & reference;
    typedef const value_type  *const_pointer;
    typedef const value_type & const_reference;

    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    // 由于在STL容器中allocator是以模板参数传入的，而对于节点型容器，
    // 如list, set, map，需要重新绑定数据结构，因而 rebind 便会用到
    template<class _Other>
    struct rebind
    {   // convert this type to allocator<_Other>
        typedef allocator<_Other> other;
    };

    pointer address(reference _Val) const
    {   // return address of mutable _Val
        return ((pointer) &(char&)_Val);
    }

    const_pointer address(const_reference _Val) const
    {   // return address of nonmutable _Val
        return ((const_pointer) &(char&)_Val);
    }

    // construct default allocator (do nothing)
    allocator() {}

    // construct by copying (do nothing)
    allocator(const allocator<_Ty>&) _THROW0(){}

    // 这里使用了 Coercion by Member Template 惯用法
    template<class _Other>
    allocator(const allocator<_Other>&) _THROW0()
    {   // construct from a related allocator (do nothing)
    }

    // 这里使用了 Coercion by Member Template 惯用法
    template<class _Other>
    allocator<_Ty>& operator=(const allocator<_Other>&)
    {   // assign from a related allocator (do nothing)
        return (*this);
    }

    // 内存释放
    void deallocate(pointer _Ptr, size_type)
    {   // deallocate object at _Ptr, ignore size
        ::operator delete(_Ptr);
    }

    // 内存分配
    pointer allocate(size_type _Count)
    {   // allocate array of _Count elements
        return (_Allocate(_Count, (pointer)0));
    }

    pointer allocate(size_type _Count, const void  *)
    {   // allocate array of _Count elements, ignore hint
        return (allocate(_Count));
    }

    void construct(pointer _Ptr, const _Ty& _Val)
    {   // construct object at _Ptr with value _Val
        _Construct(_Ptr, _Val);
    }

    // placement new
    void construct(pointer _Ptr, _Ty&& _Val)
    {   // construct object at _Ptr with value _Val
        ::new ((void  *)_Ptr) _Ty(_STD forward<_Ty>(_Val));
    }

    template<class _Other>
    void construct(pointer _Ptr, _Other&& _Val)
    {   // construct object at _Ptr with value _Val
        ::new ((void  *)_Ptr) _Ty(_STD forward<_Other>(_Val));
    }

    // 对象销毁
    void destroy(pointer _Ptr)
    {   // destroy object at _Ptr
        _Destroy(_Ptr);
    }

    size_t max_size() const _THROW0()
    {   // estimate maximum array size
        size_t _Count = (size_t)(-1) / sizeof (_Ty);
        return (0 < _Count ? _Count : 1);
    }
};


template<> class allocator<void> { 

public:
    typedef void T;
    typedef T* pointer;
    typedef const T  *const_pointer;
    typedef T value_type;

    // rebind 必须重定义
    template<class _Other>
    struct rebind
    {   // convert this type to an allocator<_Other>
        typedef allocator<_Other> other;
    };

    allocator(){}

    allocator(const allocator<_Ty>&){}

    template<class _Other>
    allocator(const allocator<_Other>&) _THROW0()
    {   // construct from related allocator (do nothing)
    }

    template<class _Other>
    allocator<_Ty>& operator=(const allocator<_Other>&)
    {   // assign from a related allocator (do nothing)
        return (*this);
    }
};

// allocator内部由于没有数据，因而总是相同的功能
template<class _Ty,
         class _Other> inline
bool operator==(const allocator<_Ty>&,
                const allocator<_Other>&) _THROW0()
{   // test for allocator equality
    return (true);
}

template<class _Ty,
         class _Other> inline
bool operator!=(const allocator<_Ty>& _Left,
                const allocator<_Other>& _Right) _THROW0()
{   // test for allocator inequality
    return (!(_Left == _Right));
}

// TEMPLATE FUNCTIONS _Cons_val AND _Dest_val
template<class _Alloc,
         class _Ty1,
         class _Ty2>
void _Cons_val(_Alloc& _Alval, _Ty1 *_Pdest, _Ty2&& _Src)
{   // construct using allocator
    _Alval.construct(_Pdest, _STD forward<_Ty2>(_Src));
}

template<class _Alloc,
         class _Ty1>
void _Dest_val(_Alloc& _Alval, _Ty1 *_Pdest)
{   // destroy using allocator
    _Alval.destroy(_Pdest);
}

#pragma pop_macro("new")

#pragma warning(pop)
#pragma pack(pop)

#endif /* RC_INVOKED */
#endif /* _XMEMORY_ */


