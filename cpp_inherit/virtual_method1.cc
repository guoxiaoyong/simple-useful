#include <iostream>
#include <typeinfo>

namespace {

struct Parent {
  Parent() {
    std::cout << "Parent created.\n";
    foo();
  };
  void say() {
    foo();
  }

  virtual void foo() {
    std::cout << "I'm parent." << std::endl;
  }
};

struct Child: public Parent {
  Child() {
    std::cout << "Child created.\n";
    foo();
  };
  virtual void foo() {
    std::cout << "I'm child." << std::endl;
  }
};

}

int main(void) {
  auto p = Parent();
  p.say();

  auto c = Child();
  c.say();

  auto& ti = typeid(Child);

  std::cout << &ti << std::endl;
  std::cout << sizeof(&ti) << std::endl;
  std::cout << ti.name() << std::endl;
  std::cout << typeid(std::string).name() << std::endl;

  return 0;
}
