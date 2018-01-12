
template <typename T>
class smart_ptr {

public:
    smart_ptr(T* p);
    ~smart_ptr();
    T& operator*();
    T* operator->();

private:
    T* ptr;
};

template <typename T>
smart_ptr<T>::smart_ptr(T* p):ptr(p) {
}

template <typename T>
smart_ptr<T>::~smart_ptr() {
    delete ptr;
}

template <typename T>
T& smart_ptr<T>::operator*() {
    return *ptr;
}

template <typename T>
T* smart_ptr<T>::operator->() {
    return ptr;
}

