// Get the address of virtual function table.
// Return it as a unsigned long.
template <typename T> unsigned long GetVPtr(T *obj) {
  auto ptr = reinterpret_cast<unsigned long *>(obj);
  return ptr[0];
}

template <typename T> unsigned long GetVPtr(T obj) {
  auto ptr = reinterpret_cast<unsigned long *>(&obj);
  return ptr[0];
}

// We need some trick here to do the conversion.
template <typename T> T UL2Ptr(unsigned long num) {
  static_assert(std::is_pointer<T>::value ||
                    std::is_member_function_pointer<T>::value,
                "type parameter must be a pointer type!");

  union DataView {
    T type_ptr;
    unsigned long number;
  };

  DataView dv;
  dv.number = num;
  return dv.type_ptr;
}

template <typename T> unsigned long Ptr2UL(T obj) {
  static_assert(std::is_pointer<T>::value ||
                    std::is_member_function_pointer<T>::value,
                "type parameter must be a pointer type!");

  union DataView {
    T obj;
    unsigned long number;
  };

  DataView dv;
  dv.obj = obj;
  return dv.number;
}

template <typename OriginalType, typename TargetType>
TargetType Ptr2Ptr(OriginalType original_ptr, TargetType target_ptr) {
  static_assert(std::is_pointer<OriginalType>::value ||
                    std::is_member_function_pointer<OriginalType>::value,
                "OriginalType must be a pointer type!");
  static_assert(std::is_pointer<TargetType>::value ||
                    std::is_member_function_pointer<TargetType>::value,
                "TargetType must be a pointer type!");

  union DataView {
    OriginalType original_ptr;
    TargetType target_ptr;
  };

  DataView dv;
  dv.original_ptr = original_ptr;
  return dv.target_ptr;
}
