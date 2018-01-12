#include <iostream>

class Top {
 public:
  int a;
};

class Left : virtual public Top {
 public:
  int b;
};

class Right : virtual public Top {
 public:
  int c;
};

class Bottom : public Left, public Right {
 public:
  int d;
};

int main(void) {
  auto bottom = Bottom();
  Top* top = (Top*)&bottom;
  Top* topL = (Left*)&bottom;
  Top* topR = (Right*)&bottom;

  std::cout << "Top object: " << top << std::endl;
  std::cout << "Top object in Left: " << topL << std::endl;
  std::cout << "Top object in Right: " << topR << std::endl;
  return 0;
}
