#include <iostream>
#include <functional>

struct Parent {
   virtual ~Parent() {
     std::cout << "Parent destroyed!\n";
   }

   virtual void say(int n) {
     std::cout << "Parent:" << n << std::endl;
   }
};

struct Child: public Parent {
  virtual ~Child() {
     std::cout << "Child destroyed!\n";
  }
  virtual void say(int n) {
     std::cout << "Child :" << n*n << std::endl;
   }
};


std::function<void(void)>
get_fun(Parent* x, int n) {
  return std::bind(&Parent::say, x, n);
}

int main(void) {
  Child c;
  get_fun(&c, 10)();

  Parent p;
  get_fun(&p, 10)();
  return 0;
}
