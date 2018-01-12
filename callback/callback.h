#ifndef BASE_CALLBACK_H_
#define BASE_CALLBACK_H_

#include <iostream>
#include <type_traits>

#define COMPILE_ASSERT(expr, msg) static_assert(expr, #msg)

// Helper classes/functions.
template <class T> struct is_non_const_reference : std::false_type {};
template <class T> struct is_non_const_reference<T &> : std::true_type {};
template <class T>
struct is_non_const_reference<const T &> : std::false_type {};

// ConstScopedInstance.
template <bool del, typename T> class ConstScopedInstance {
public:
  ConstScopedInstance(const T *t) : instance_(t) {}
  ~ConstScopedInstance() {
    std::cout << "instance deleted! " << instance_ << std::endl;
    if (del)
      delete instance_;
  }

private:
  const T *instance_;
};

// Traits classes (ParamTraits, FunctionTraits, CallbackTraits).
template <typename T> struct ParamTraits {
  typedef const T &ForwardType;
  typedef T StorageType;
};

template <typename T> struct ParamTraits<T &> {
  typedef T &ForwardType;
  typedef T StorageType;
};

template <typename T> struct ParamTraits<const T &> {
  typedef const T &ForwardType;
  typedef T StorageType;
};

// FunctionTraits.
template <typename Sig> struct FunctionTraits;

// Function: Arity 0.
template <typename R> struct FunctionTraits<R (*)()> {
  typedef R (*NormalizedSig)();
  typedef std::false_type IsMethod;

  typedef R Return;
};

// Method: Arity 0.
template <typename R, typename T> struct FunctionTraits<R (T::*)()> {
  typedef R (T::*NormalizedSig)();
  typedef std::true_type IsMethod;

  typedef R Return;

  // Target type for each bound parameter.
  typedef T B1;
};

// Const Method: Arity 0.
template <typename R, typename T> struct FunctionTraits<R (T::*)() const> {
  typedef R (T::*NormalizedSig)();
  typedef std::true_type IsMethod;

  typedef R Return;

  // Target type for each bound parameter.
  typedef T B1;
};

// Function: Arity 1.
template <typename R, typename X1> struct FunctionTraits<R (*)(X1)> {
  typedef R (*NormalizedSig)(X1);
  typedef std::false_type IsMethod;

  typedef R Return;

  // Target type for each bound parameter.
  typedef X1 B1;
};

// Method: Arity 1.
template <typename R, typename T, typename X1>
struct FunctionTraits<R (T::*)(X1)> {
  typedef R (T::*NormalizedSig)(X1);
  typedef std::true_type IsMethod;

  typedef R Return;

  // Target type for each bound parameter.
  typedef T B1;
  typedef X1 B2;
};

// Const Method: Arity 1.
template <typename R, typename T, typename X1>
struct FunctionTraits<R (T::*)(X1) const> {
  typedef R (T::*NormalizedSig)(X1);
  typedef std::true_type IsMethod;

  typedef R Return;

  // Target type for each bound parameter.
  typedef T B1;
  typedef X1 B2;
};

// Function: Arity 2.
template <typename R, typename X1, typename X2>
struct FunctionTraits<R (*)(X1, X2)> {
  typedef R (*NormalizedSig)(X1, X2);
  typedef std::false_type IsMethod;

  typedef R Return;

  // Target type for each bound parameter.
  typedef X1 B1;
  typedef X2 B2;
};

// Method: Arity 2.
template <typename R, typename T, typename X1, typename X2>
struct FunctionTraits<R (T::*)(X1, X2)> {
  typedef R (T::*NormalizedSig)(X1, X2);
  typedef std::true_type IsMethod;

  typedef R Return;

  // Target type for each bound parameter.
  typedef T B1;
  typedef X1 B2;
  typedef X2 B3;
};

// Const Method: Arity 2.
template <typename R, typename T, typename X1, typename X2>
struct FunctionTraits<R (T::*)(X1, X2) const> {
  typedef R (T::*NormalizedSig)(X1, X2);
  typedef std::true_type IsMethod;

  typedef R Return;

  // Target type for each bound parameter.
  typedef T B1;
  typedef X1 B2;
  typedef X2 B3;
};

// CallbackTraits.
// The first template parameter is the number of bound parameters.
template <int B, typename Sig> struct CallbackTraits;

// Function: Arity 0 -> 0.
template <typename R> struct CallbackTraits<0, R (*)()> {
  typedef R (*CallbackSig)(void);
  typedef std::false_type IsMethod;
  typedef R Return;
};

// Function: Arity 1 -> 1.
template <typename R, typename X1> struct CallbackTraits<0, R (*)(X1)> {
  typedef R (*CallbackSig)(X1);
  typedef std::false_type IsMethod;
  typedef R Return;
};

// Function: Arity 1 -> 0.
template <typename R, typename X1> struct CallbackTraits<1, R (*)(X1)> {
  typedef R (*CallbackSig)(void);
  typedef std::false_type IsMethod;
  typedef R Return;
};

// Method: Arity 0 -> 0.
template <typename R, typename T> struct CallbackTraits<1, R (T::*)()> {
  typedef R (*CallbackSig)(void);
  typedef std::true_type IsMethod;
  typedef R Return;
};

// Const Method: Arity 0 -> 0.
template <typename R, typename T> struct CallbackTraits<1, R (T::*)() const> {
  typedef R (*CallbackSig)(void);
  typedef std::true_type IsMethod;
  typedef R Return;
};

// Function: Arity 2 -> 2.
template <typename R, typename X1, typename X2>
struct CallbackTraits<0, R (*)(X1, X2)> {
  typedef R (*CallbackSig)(X1, X2);
  typedef std::false_type IsMethod;
  typedef R Return;
};

// Function: Arity 2 -> 1.
template <typename R, typename X1, typename X2>
struct CallbackTraits<1, R (*)(X1, X2)> {
  typedef R (*CallbackSig)(X2);
  typedef std::false_type IsMethod;
  typedef R Return;
};

// Method: Arity 1 -> 1.
template <typename R, typename T, typename X1>
struct CallbackTraits<1, R (T::*)(X1)> {
  typedef R (*CallbackSig)(X1);
  typedef std::true_type IsMethod;
  typedef R Return;
};

// Const Method: Arity 1 -> 1.
template <typename R, typename T, typename X1>
struct CallbackTraits<1, R (T::*)(X1) const> {
  typedef R (*CallbackSig)(X1);
  typedef std::true_type IsMethod;
  typedef R Return;
};

// Function: Arity 2 -> 0.
template <typename R, typename X1, typename X2>
struct CallbackTraits<2, R (*)(X1, X2)> {
  typedef R (*CallbackSig)(void);
  typedef std::false_type IsMethod;
  typedef R Return;
};

// Method: Arity 1 -> 0.
template <typename R, typename T, typename X1>
struct CallbackTraits<2, R (T::*)(X1)> {
  typedef R (*CallbackSig)(void);
  typedef std::true_type IsMethod;
  typedef R Return;
};

// Const Method: Arity 1 -> 0.
template <typename R, typename T, typename X1>
struct CallbackTraits<2, R (T::*)(X1) const> {
  typedef R (*CallbackSig)(void);
  typedef std::true_type IsMethod;
  typedef R Return;
};

// Method: Arity 2 -> 2.
template <typename R, typename T, typename X1, typename X2>
struct CallbackTraits<1, R (T::*)(X1, X2)> {
  typedef R (*CallbackSig)(X1, X2);
  typedef std::true_type IsMethod;
  typedef R Return;
};

// Const Method: Arity 2 -> 2.
template <typename R, typename T, typename X1, typename X2>
struct CallbackTraits<1, R (T::*)(X1, X2) const> {
  typedef R (*CallbackSig)(X1, X2);
  typedef std::true_type IsMethod;
  typedef R Return;
};

// Method: Arity 2 -> 1.
template <typename R, typename T, typename X1, typename X2>
struct CallbackTraits<2, R (T::*)(X1, X2)> {
  typedef R (*CallbackSig)(X2);
  typedef std::true_type IsMethod;
  typedef R Return;
};

// Const Method: Arity 2 -> 1.
template <typename R, typename T, typename X1, typename X2>
struct CallbackTraits<2, R (T::*)(X1, X2) const> {
  typedef R (*CallbackSig)(X2);
  typedef std::true_type IsMethod;
  typedef R Return;
};

// Method: Arity 2 -> 0.
template <typename R, typename T, typename X1, typename X2>
struct CallbackTraits<3, R (T::*)(X1, X2)> {
  typedef R (*CallbackSig)(void);
  typedef std::true_type IsMethod;
  typedef R Return;
};

// Const Method: Arity 2 -> 0.
template <typename R, typename T, typename X1, typename X2>
struct CallbackTraits<3, R (T::*)(X1, X2) const> {
  typedef R (*CallbackSig)(void);
  typedef std::true_type IsMethod;
  typedef R Return;
};

// InvokerStorageBase.
class InvokerStorageBase {
public:
  typedef void (*InvokeFuncStorage)(void);
  virtual ~InvokerStorageBase() {}
  virtual bool IsRepeatable() const = 0;
  inline InvokerStorageBase *invoker_storage() { return this; }
  InvokeFuncStorage do_invoke_;
};

// InvokerN, N is the number of bound parameters.
// StorageType can be InvokeStorageN.
// InvokerN has no data member, it contains a static method which invokes
// the function/methods save in InvokerStoarageN.
// It also save this method's call signature in the |DoInvokerType|.
template <bool del, typename StorageType, typename NormalizedSig>
struct Invoker0;

// Function: Arity 0 -> 0.
template <bool del, typename StorageType, typename R>
struct Invoker0<del, StorageType, R (*)()> {
  typedef R (*DoInvokeType)(const InvokerStorageBase *);

  static R DoInvoke(const InvokerStorageBase *base) {
    ConstScopedInstance<del, InvokerStorageBase> scoped_instance(base);
    const StorageType *invoker = static_cast<const StorageType *>(base);
    return (invoker->f_)();
  }
};

// Function: Arity 1 -> 1.
template <bool del, typename StorageType, typename R, typename X1>
struct Invoker0<del, StorageType, R (*)(X1)> {
  typedef R (*DoInvokeType)(const InvokerStorageBase *,
                            typename ParamTraits<X1>::ForwardType);

  static R DoInvoke(const InvokerStorageBase *base,
                    typename ParamTraits<X1>::ForwardType x1) {
    ConstScopedInstance<del, InvokerStorageBase> scoped_instance(base);
    const StorageType *invoker = static_cast<const StorageType *>(base);
    return (invoker->f_)(x1);
  }
};

// Function: Arity 2 -> 2.
template <bool del, typename StorageType, typename R, typename X1, typename X2>
struct Invoker0<del, StorageType, R (*)(X1, X2)> {
  typedef R (*DoInvokeType)(const InvokerStorageBase *,
                            typename ParamTraits<X1>::ForwardType,
                            typename ParamTraits<X2>::ForwardType);

  static R DoInvoke(const InvokerStorageBase *base,
                    typename ParamTraits<X1>::ForwardType x1,
                    typename ParamTraits<X2>::ForwardType x2) {
    ConstScopedInstance<del, InvokerStorageBase> scoped_instance(base);
    const StorageType *invoker = static_cast<const StorageType *>(base);
    return (invoker->f_)(x1, x2);
  }
};

template <bool del, typename StorageType, typename NormalizedSig>
struct Invoker1;

// Function: Arity 1 -> 0.
template <bool del, typename StorageType, typename R, typename X1>
struct Invoker1<del, StorageType, R (*)(X1)> {
  typedef R (*DoInvokeType)(const InvokerStorageBase *);

  static R DoInvoke(const InvokerStorageBase *base) {
    ConstScopedInstance<del, InvokerStorageBase> scoped_instance(base);
    const StorageType *invoker = static_cast<const StorageType *>(base);
    return (invoker->f_)(invoker->p1_);
  }
};

// Method: Arity 0 -> 0.
template <bool del, typename StorageType, typename R, typename T>
struct Invoker1<del, StorageType, R (T::*)()> {
  typedef R (*DoInvokeType)(const InvokerStorageBase *);

  static R DoInvoke(const InvokerStorageBase *base) {
    ConstScopedInstance<del, InvokerStorageBase> scoped_instance(base);
    const StorageType *invoker = static_cast<const StorageType *>(base);
    return ((invoker->p1_)->*invoker->f_)();
  }
};

// Function: Arity 2 -> 1.
template <bool del, typename StorageType, typename R, typename X1, typename X2>
struct Invoker1<del, StorageType, R (*)(X1, X2)> {
  typedef R (*DoInvokeType)(const InvokerStorageBase *,
                            typename ParamTraits<X2>::ForwardType);

  static R DoInvoke(const InvokerStorageBase *base,
                    typename ParamTraits<X2>::ForwardType x2) {
    ConstScopedInstance<del, InvokerStorageBase> scoped_instance(base);
    const StorageType *invoker = static_cast<const StorageType *>(base);
    return (invoker->f_)(invoker->p1_, x2);
  }
};

// Method: Arity 1 -> 1.
template <bool del, typename StorageType, typename R, typename T, typename X1>
struct Invoker1<del, StorageType, R (T::*)(X1)> {
  typedef R (*DoInvokeType)(const InvokerStorageBase *,
                            typename ParamTraits<X1>::ForwardType);

  static R DoInvoke(const InvokerStorageBase *base,
                    typename ParamTraits<X1>::ForwardType x1) {
    ConstScopedInstance<del, InvokerStorageBase> scoped_instance(base);
    const StorageType *invoker = static_cast<const StorageType *>(base);
    return ((invoker->p1_)->*invoker->f_)(x1);
  }
};

// Method: Arity 2 -> 2.
template <bool del, typename StorageType, typename R, typename T, typename X1,
          typename X2>
struct Invoker1<del, StorageType, R (T::*)(X1, X2)> {
  typedef R (*DoInvokeType)(const InvokerStorageBase *,
                            typename ParamTraits<X1>::ForwardType,
                            typename ParamTraits<X2>::ForwardType);

  static R DoInvoke(const InvokerStorageBase *base,
                    typename ParamTraits<X1>::ForwardType x1,
                    typename ParamTraits<X2>::ForwardType x2) {
    ConstScopedInstance<del, InvokerStorageBase> scoped_instance(base);
    const StorageType *invoker =
        static_cast<const StorageType *>(base); // Is it safe? dynamic_cast
    return ((invoker->p1_)->*invoker->f_)(x1, x2);
  }
};

template <bool del, typename StorageType, typename NormalizedSig>
struct Invoker2;

// Function: Arity 2 -> 0.
template <bool del, typename StorageType, typename R, typename X1, typename X2>
struct Invoker2<del, StorageType, R (*)(X1, X2)> {
  typedef R (*DoInvokeType)(const InvokerStorageBase *);

  static R DoInvoke(const InvokerStorageBase *base) {
    ConstScopedInstance<del, InvokerStorageBase> scoped_instance(base);
    const StorageType *invoker = static_cast<const StorageType *>(base);
    return (invoker->f_)(invoker->p1_, invoker->p2_);
  }
};

// Method: Arity 1 -> 0.
template <bool del, typename StorageType, typename R, typename T, typename X1>
struct Invoker2<del, StorageType, R (T::*)(X1)> {
  typedef R (*DoInvokeType)(const InvokerStorageBase *);

  static R DoInvoke(const InvokerStorageBase *base) {
    ConstScopedInstance<del, InvokerStorageBase> scoped_instance(base);
    const StorageType *invoker = static_cast<const StorageType *>(base);
    return ((invoker->p1_)->*invoker->f_)(invoker->p2_);
  }
};

// Method: Arity 2 -> 1.
template <bool del, typename StorageType, typename R, typename T, typename X1,
          typename X2>
struct Invoker2<del, StorageType, R (T::*)(X1, X2)> {
  typedef R (*DoInvokeType)(const InvokerStorageBase *,
                            typename ParamTraits<X2>::ForwardType);

  static R DoInvoke(const InvokerStorageBase *base,
                    typename ParamTraits<X2>::ForwardType x2) {
    ConstScopedInstance<del, InvokerStorageBase> scoped_instance(base);
    const StorageType *invoker = static_cast<const StorageType *>(base);
    return ((invoker->p1_)->*invoker->f_)(invoker->p2_, x2);
  }
};

// InvokerStorageN.
// N is the number of bound parameters.
template <bool del, typename Sig>
class InvokerStorage0 : public InvokerStorageBase {
public:
  typedef InvokerStorage0 StorageType;
  typedef FunctionTraits<Sig> TargetTraits;
  typedef typename TargetTraits::IsMethod IsMethod;
  typedef Sig Signature;

  typedef Invoker0<del, StorageType, typename TargetTraits::NormalizedSig>
      Invoker;

  InvokerStorage0(Sig f) : f_(f) {
    do_invoke_ = reinterpret_cast<InvokeFuncStorage>(&Invoker::DoInvoke);
  }

  virtual ~InvokerStorage0() {}
  virtual bool IsRepeatable() const { return !del; }
  Sig f_;
};

template <bool del, typename Sig, typename P1>
class InvokerStorage1 : public InvokerStorageBase {
public:
  typedef InvokerStorage1 StorageType;
  typedef FunctionTraits<Sig> TargetTraits;
  typedef typename TargetTraits::IsMethod IsMethod;
  typedef Sig Signature;
  typedef ParamTraits<P1> P1Traits;

  typedef Invoker1<del, StorageType, typename TargetTraits::NormalizedSig>
      Invoker;

  // We disallow binding of an array as the method's target object.
  COMPILE_ASSERT(!IsMethod::value || !std::is_array<P1>::value,
                 first_bound_argument_to_method_cannot_be_array);

  // Do not allow binding a non-const reference parameter. Non-const reference
  // parameters are disallowed by the Google style guide.  Also, binding a
  // non-const reference parameter can make for subtle bugs because the
  // invoked function will receive a reference to the stored copy of the
  // argument and not the original.
  COMPILE_ASSERT(!(is_non_const_reference<typename TargetTraits::B1>::value),
                 do_not_bind_functions_with_nonconst_ref);

  InvokerStorage1(Sig f, const P1 &p1)
      : f_(f), p1_(static_cast<typename ParamTraits<P1>::StorageType>(p1)) {
    do_invoke_ = reinterpret_cast<InvokeFuncStorage>(&Invoker::DoInvoke);
  }

  virtual ~InvokerStorage1() {}
  virtual bool IsRepeatable() const { return !del; }
  Sig f_;
  typename ParamTraits<P1>::StorageType p1_;
};

template <bool del, typename Sig, typename P1, typename P2>
class InvokerStorage2 : public InvokerStorageBase {
public:
  typedef InvokerStorage2 StorageType;
  typedef FunctionTraits<Sig> TargetTraits;
  typedef typename TargetTraits::IsMethod IsMethod;
  typedef Sig Signature;
  typedef ParamTraits<P1> P1Traits;
  typedef ParamTraits<P2> P2Traits;

  typedef Invoker2<del, StorageType, typename TargetTraits::NormalizedSig>
      Invoker;

  // We disallow binding of an array as the method's target object.
  COMPILE_ASSERT(!IsMethod::value || !std::is_array<P1>::value,
                 first_bound_argument_to_method_cannot_be_array);

  // Do not allow binding a non-const reference parameter. Non-const reference
  // parameters are disallowed by the Google style guide.  Also, binding a
  // non-const reference parameter can make for subtle bugs because the
  // invoked function will receive a reference to the stored copy of the
  // argument and not the original.
  COMPILE_ASSERT(!(is_non_const_reference<typename TargetTraits::B1>::value ||
                   is_non_const_reference<typename TargetTraits::B2>::value),
                 do_not_bind_functions_with_nonconst_ref);

  InvokerStorage2(Sig f, const P1 &p1, const P2 &p2)
      : f_(f), p1_(static_cast<typename ParamTraits<P1>::StorageType>(p1)),
        p2_(static_cast<typename ParamTraits<P2>::StorageType>(p2)) {

    do_invoke_ = reinterpret_cast<InvokeFuncStorage>(&Invoker::DoInvoke);
  }

  virtual ~InvokerStorage2() {}
  virtual bool IsRepeatable() const { return !del; }
  Sig f_;
  typename ParamTraits<P1>::StorageType p1_;
  typename ParamTraits<P2>::StorageType p2_;
};

// Callback.
template <typename Sig> class Callback;

template <typename R> class Callback<R(void)> {
public:
  typedef R (*PolymorphicInvoke)(const InvokerStorageBase *);

  virtual ~Callback() {}

  R Run() const {
    const InvokerStorageBase *invoker_storage = bound_invoker_storage();
    const PolymorphicInvoke f =
        reinterpret_cast<const PolymorphicInvoke>(invoker_storage->do_invoke_);
    return f(invoker_storage);
  }

  bool IsRepeatable() const { return bound_invoker_storage()->IsRepeatable(); }

protected:
  inline void BindInvokerStorage(InvokerStorageBase *invoker_storage) {
    invoker_storage_ = invoker_storage;
  }

private:
  inline const InvokerStorageBase *bound_invoker_storage() const {
    return invoker_storage_;
  }
  inline InvokerStorageBase *bound_invoker_storage() {
    return invoker_storage_;
  }
  InvokerStorageBase *invoker_storage_;
};

template <typename R, typename A1> class Callback<R(A1)> {
public:
  typedef R (*PolymorphicInvoke)(const InvokerStorageBase *,
                                 typename ParamTraits<A1>::ForwardType);

  virtual ~Callback() {}

  R Run(typename ParamTraits<A1>::ForwardType a1) const {
    const InvokerStorageBase *invoker_storage = bound_invoker_storage();
    const PolymorphicInvoke f =
        reinterpret_cast<const PolymorphicInvoke>(invoker_storage->do_invoke_);
    return f(invoker_storage, a1);
  }

  bool IsRepeatable() const { return bound_invoker_storage()->IsRepeatable(); }

protected:
  inline void BindInvokerStorage(InvokerStorageBase *invoker_storage) {
    invoker_storage_ = invoker_storage;
  }

private:
  inline const InvokerStorageBase *bound_invoker_storage() const {
    return invoker_storage_;
  }
  inline InvokerStorageBase *bound_invoker_storage() {
    return invoker_storage_;
  }
  InvokerStorageBase *invoker_storage_;
};

template <typename R, typename A1, typename A2> class Callback<R(A1, A2)> {
public:
  typedef R (*PolymorphicInvoke)(const InvokerStorageBase *,
                                 typename ParamTraits<A1>::ForwardType,
                                 typename ParamTraits<A2>::ForwardType);

  virtual ~Callback() {}

  R Run(typename ParamTraits<A1>::ForwardType a1,
        typename ParamTraits<A2>::ForwardType a2) const {
    const InvokerStorageBase *invoker_storage = bound_invoker_storage();
    const PolymorphicInvoke f =
        reinterpret_cast<const PolymorphicInvoke>(invoker_storage->do_invoke_);
    return f(invoker_storage, a1, a2);
  }

  bool IsRepeatable() const { return bound_invoker_storage()->IsRepeatable(); }

protected:
  inline void BindInvokerStorage(InvokerStorageBase *invoker_storage) {
    invoker_storage_ = invoker_storage;
  }

private:
  inline const InvokerStorageBase *bound_invoker_storage() const {
    return invoker_storage_;
  }
  inline InvokerStorageBase *bound_invoker_storage() {
    return invoker_storage_;
  }
  InvokerStorageBase *invoker_storage_;
};

// CallbackFunction.
template <typename Sig> class CallbackFunction;

template <typename R>
class CallbackFunction<R (*)(void)> : public Callback<R(void)> {
public:
  virtual ~CallbackFunction() {}
};
template <typename R, typename A1>
class CallbackFunction<R (*)(A1)> : public Callback<R(A1)> {
public:
  virtual ~CallbackFunction() {}
};
template <typename R, typename A1, typename A2>
class CallbackFunction<R (*)(A1, A2)> : public Callback<R(A1, A2)> {
public:
  virtual ~CallbackFunction() {}
};

// LooseBoundCallbackStorageN
template <bool del, typename Sig>
class LooseBoundCallbackStorage0
    : public CallbackFunction<typename CallbackTraits<0, Sig>::CallbackSig>,
      public InvokerStorage0<del, Sig> {
public:
  typedef typename CallbackTraits<0, Sig>::CallbackSig CallbackSigniture;
  typedef CallbackFunction<CallbackSigniture> CallbackType;
  typedef InvokerStorage0<del, Sig> InvokerStorage;
  typedef typename InvokerStorage::Signature Signature;

  LooseBoundCallbackStorage0(Sig f) : InvokerStorage(f) {
    this->BindInvokerStorage(this);
  }
  virtual ~LooseBoundCallbackStorage0() {}
};

template <bool del, typename Sig, typename P1>
class LooseBoundCallbackStorage1
    : public CallbackFunction<typename CallbackTraits<1, Sig>::CallbackSig>,
      public InvokerStorage1<del, Sig, P1> {
public:
  typedef typename CallbackTraits<1, Sig>::CallbackSig CallbackSigniture;
  typedef CallbackFunction<CallbackSigniture> CallbackType;
  typedef InvokerStorage1<del, Sig, P1> InvokerStorage;
  typedef typename InvokerStorage::Signature Signature;

  LooseBoundCallbackStorage1(Sig f, const P1 &p1) : InvokerStorage(f, p1) {
    this->BindInvokerStorage(this);
  }
  virtual ~LooseBoundCallbackStorage1() {}
};

template <bool del, typename Sig, typename P1, typename P2>
class LooseBoundCallbackStorage2
    : public CallbackFunction<typename CallbackTraits<2, Sig>::CallbackSig>,
      public InvokerStorage2<del, Sig, P1, P2> {
public:
  typedef typename CallbackTraits<2, Sig>::CallbackSig CallbackSigniture;
  typedef CallbackFunction<CallbackSigniture> CallbackType;
  typedef InvokerStorage2<del, Sig, P1, P2> InvokerStorage;
  typedef typename InvokerStorage::Signature Signature;

  LooseBoundCallbackStorage2(Sig f, const P1 &p1, const P2 &p2)
      : InvokerStorage(f, p1, p2) {
    this->BindInvokerStorage(this);
  }
  virtual ~LooseBoundCallbackStorage2() {}
};

// NewCallback.

template <typename Sig>
CallbackFunction<typename CallbackTraits<0, Sig>::CallbackSig> *
NewCallback(Sig f) {
  return new LooseBoundCallbackStorage0<true, Sig>(f);
}

template <typename Sig>
CallbackFunction<typename CallbackTraits<0, Sig>::CallbackSig> *
NewPermanentCallback(Sig f) {
  return new LooseBoundCallbackStorage0<false, Sig>(f);
}

template <typename Sig, typename P1>
CallbackFunction<typename CallbackTraits<1, Sig>::CallbackSig> *
NewCallback(Sig f, const P1 &p1) {
  return new LooseBoundCallbackStorage1<true, Sig, P1>(f, p1);
}

template <typename Sig, typename P1>
CallbackFunction<typename CallbackTraits<1, Sig>::CallbackSig> *
NewPermanentCallback(Sig f, const P1 &p1) {
  return new LooseBoundCallbackStorage1<false, Sig, P1>(f, p1);
}

template <typename Sig, typename P1, typename P2>
CallbackFunction<typename CallbackTraits<2, Sig>::CallbackSig> *
NewCallback(Sig f, const P1 &p1, const P2 &p2) {
  return new LooseBoundCallbackStorage2<true, Sig, P1, P2>(f, p1, p2);
}

template <typename Sig, typename P1, typename P2>
CallbackFunction<typename CallbackTraits<2, Sig>::CallbackSig> *
NewPermanentCallback(Sig f, const P1 &p1, const P2 &p2) {
  return new LooseBoundCallbackStorage2<false, Sig, P1, P2>(f, p1, p2);
}

typedef Callback<void(void)> Closure;

extern Closure *NewBarrierClosure(int N, Closure *done_closure);
extern void DoNothing();
extern void ClosureRunner(Closure *closure);

// DeletePointerClosure can be used to create a closure that calls delete
// on a pointer.  Here is an example:
//
//  thread->Add(DeletePointerClosure(expensive_to_delete));
//
template <typename T> void DeletePointer(T *p) { delete p; }

template <typename T> Closure *DeletePointerClosure(T *p) {
  return NewCallback(&DeletePointer<T>, p);
}

#endif // BASE_CALLBACK_H_
