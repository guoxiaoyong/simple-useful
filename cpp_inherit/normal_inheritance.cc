#include <iostream>

class Top {
 public:
  int a;
};

class Left : public Top {
 public:
  int b;
};

class Right : public Top {
 public:
  int c;
};

class Bottom : public Left, public Right {
 public:
  int d;
};

int main(void) {
  auto bottom = Bottom();
  // Following line will generate error: ‘Top’ is an ambiguous base of ‘Bottom’
  // Top* top = (Top*)&bottom;
  Top* topL = (Left*)&bottom;
  Top* topR = (Right*)&bottom;

  std::cout << "Top object in Left: " << topL << std::endl;
  std::cout << "Top object in Right: " << topR << std::endl;
  return 0;
}
