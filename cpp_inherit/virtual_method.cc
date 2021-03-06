#include <iostream>
#include <type_traits>

#include "pointer_util.h"

// General function type that can be reinterpreted to other function types.
typedef void (*Function)(void);
typedef void (*Method)(void *);

class Box {
public:
  void set(int len) { length = len; }
  virtual void open() { std::cout << this << " : open" << std::endl; }
  virtual void close() { std::cout << this << " : close" << std::endl; }
  virtual void put() { std::cout << this << " : put" << std::endl; }
  virtual void take() { std::cout << this << " : take" << std::endl; }
  virtual void other1() { std::cout << this << " : other1" << std::endl; }
  virtual void other2() { std::cout << this << " : other2" << std::endl; }

private:
  int height;
  int width;
  int length;
};

class Pencil {
public:
  virtual void write() { std::cout << this << " : pencil write" << std::endl; }
};

class PencilBox : public Box, public Pencil {
  virtual void open() { std::cout << this << ": open pencil box" << std::endl; }
};

union MView {
  Function function;
  Method method;
  unsigned long number;
};

int fun() { return 12; }

int main(void) {
  auto box = Box();

  std::cout << "Call open/close directly.\n";
  box.open();
  box.close();
  std::cout << "================\n";

  printf("Print virtual method address\n");
  printf("open method address %d\n", &Box::open);
  printf("close method address %d\n", &Box::close);
  printf("put method address %d\n", &Box::put);
  printf("take method address %d\n", &Box::take);
  printf("other1 method address %d\n", &Box::other1);
  printf("other2 method address %d\n", &Box::other2);
  printf("================\n");

  std::cout << Ptr2UL(&Box::open) << std::endl;
  std::cout << Ptr2UL(&Box::close) << std::endl;
  std::cout << Ptr2UL(&Box::put) << std::endl;
  std::cout << Ptr2UL(&Box::take) << std::endl;

  auto vtbl = GetVPtr(&box);
  printf("vtable address + 16: %x\n", vtbl);

  auto vtbl_ptr = UL2Ptr<unsigned long *>(vtbl);
  printf("%x\n", vtbl_ptr[0]);
  printf("%x\n", vtbl_ptr[1]);

  UL2Ptr<Method>(vtbl_ptr[0])(&box);
  UL2Ptr<Method>(vtbl_ptr[1])(&box);
  UL2Ptr<Method>(vtbl_ptr[2])(&box);
  UL2Ptr<Method>(vtbl_ptr[3])(&box);
  UL2Ptr<Method>(vtbl_ptr[4])(&box);
  UL2Ptr<Method>(vtbl_ptr[5])(&box);

  printf("%x\n", UL2Ptr<Method>(vtbl_ptr[6]));

  // If you look at the assembly generated by gcc,
  // you will see this in the constructor:
  //   movq  vtable for Box+16, (%rax)
  // That means the vptr in the object points to the vtable plus 16 bytes
  // offset. The are 2 pointers leading the vtable: one for the multiple
  // inheritance top pointer and another for RTTI.
  // reinterpret_cast<Method>(vfun_ptr)(&box);

  return 0;
}

void cast() {
  PencilBox pb;
  auto ptr = dynamic_cast<Pencil *>(&pb);
  auto ptr1 = dynamic_cast<PencilBox *>(ptr);
  std::cout << ptr << std::endl;
  std::cout << ptr1 << std::endl;
}
