#include <iostream>

namespace {

class Mod {
public:
  Mod() { std::cout << __FILE__ << ": Object created!" << std::endl; }

  ~Mod() { std::cout << __FILE__ << ": Object destroyed!" << std::endl; }
};

static Mod mod;
}
