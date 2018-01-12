#include <exception>
#include <typeinfo>
#include <utility>

class any {
 private:
  struct placeholder {
    virtual ~placeholder() {}
    virtual const std::type_info& type() const = 0;
    virtual placeholder* clone() const = 0;
  };

  template<typename value_type>
  struct holder: public placeholder {
    holder(const value_type& value):
        held(value) {}

    virtual const std::type_info& type() const {
      return typeid(value_type);
    }

    virtual placeholder* clone() const {
      return new holder(held);
    }

    value_type held;
  };

 public:
  template <typename T>
  any(const T& value):
      content(new holder<T>(value)) {}

  ~any() {
    delete content;
  }

  template <typename T>
  any(const any& t):content(t.content ? t.content->clone():0) {}

  const std::type_info& type() const {
    return content ? content->type():typeid(void);
  }

  any& swap(any& rhs) {
    std::swap(content, rhs.content);
    return *this;
  }

  template<typename ValueType>
  any& operator=(const ValueType& rhs) {
    any(rhs).swap(*this);
    return *this;
  }

  any& operator=(any rhs) {
    rhs.swap(*this);
    return *this;
  }

  bool empty() const {
    return !content;
  }

  template<typename T>
  friend T any_cast(const any&);

  template<typename ValueType>
  friend ValueType * unsafe_any_cast(any *);

 private:
  placeholder* content;
};

template<typename T>
T any_cast(const any& any_data) {
  if (any_data.content->type() == typeid(T)) {
    return dynamic_cast<any::holder<T>*>(any_data.content)->held;
  } else {
    throw std::exception();
  }
}
