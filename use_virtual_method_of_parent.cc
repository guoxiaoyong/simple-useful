#include <iostream>

struct Father {
  virtual void print() {
    std::cout << "father" << std::endl;
  }
};

struct Child: public Father {
  void print() override {
    Father::print();
    std::cout << "child" << std::endl;
  }
};

int main(void) {
  Child c;
  c.print();
  return 0;
}
