#include <iostream>
#include <typeinfo>

#include "pointer_util.h"

struct Father {
  virtual void say() {
    std::cout << "I'm Father!\n";
  }

  virtual void fmethod() {
    std::cout << "fmethod\n";
  }

  int a;
  int b;
};

struct Mother {
  virtual void say() {
    std::cout << "I'm Mother!\n";
  }

  virtual void mmethod() {
    std::cout << "mmethod\n";
  }

  int a;
  int b;
};

struct Child: public Father, public Mother {
  virtual void say() {
    std::cout << "I'm Child.\n";
  }

  virtual void cmethod() {
    std::cout << "mmethod\n";
  }
};

int main(void) {
  // Find the address of typeinfo for |Father|.
  Father f;
  auto vtbl_num = GetVPtr(f);
  auto vtbl_ptr = UL2Ptr<unsigned long*>(vtbl_num);

  auto& ti = typeid(Father);
  auto typeinfo_num = Ptr2UL(&ti);
  std::cout << typeinfo_num << std::endl;
  std::cout << *(vtbl_ptr-1) << std::endl;
  std::cout << "We got the same value (under GCC)" << std::endl;
  std::cout << "size of typeinfo: " << sizeof(ti) << std::endl;

  // Assign child object to Mother reference type.
  int a = 2;
  int& b = a;
  std::cout << "size of int: " << sizeof(b) << std::endl;

  Child child;
  Mother& mother = child;
  Mother m;
  mother.say();

  // Check address of child and mother.
  std::cout << &child << std::endl;
  std::cout << &mother << std::endl;
  std::cout << dynamic_cast<Child*>(&mother) << std::endl;
  std::cout << dynamic_cast<Child*>(&m) << std::endl;

  // Check typeid
  std::cout << typeid(mother).name() << std::endl;
  std::cout << typeid(m).name() << std::endl;

  std::cout << "child vtbl address: " << GetVPtr(&child) << std::endl;
  std::cout << "mother vtbl address: " << GetVPtr(&mother) << std::endl;

  vtbl_num = GetVPtr(&mother);
  vtbl_ptr = UL2Ptr<unsigned long*>(vtbl_num);
  long offset_to_top = static_cast<long>(*(vtbl_ptr-2));
  std::cout << "offset_to_top: " << offset_to_top << std::endl;

  // Check virtual function address
  std::cout << Ptr2UL(&Child::fmethod) << std::endl;
  std::cout << Ptr2UL(&Father::fmethod) << std::endl;

  std::cout << Ptr2UL(&Child::mmethod) << std::endl;
  std::cout << Ptr2UL(&Mother::mmethod) << std::endl;

  child.mmethod();
  return 0;
}
