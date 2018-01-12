#include <iostream>

struct Parent {
  Parent() {
    std::cout << "Parent created!\n";
  }

  Parent(const Parent& p) {
    std::cout << "copy constructor called!\n";
  }

  Parent& operator=(const Parent& p) {
    std::cout << "assignment constructor called!\n";
  }

  virtual void say() {
    std::cout << "I'm parent.\n";
  }
};

struct Child : public Parent {
  Child() {
    std::cout << "Child created!\n";
  }

  virtual void say() {
    std::cout << "I'm child.\n";
  }
};

void fun(Parent p) {
  p.say();
}

int main(void) {

  auto p = Parent();
  p.say();
  printf("==============\n");

  auto c = Child();
  c.say();
  printf("==============\n");

  Parent p1(c);
  p1.say();
  printf("==============\n");

  Parent p2;
  p2 = c;
  p2.say();
  printf("==============\n");

  fun(c);
  return 0;
}
