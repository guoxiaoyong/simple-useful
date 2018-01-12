#include <iostream>
#include <type_traits>

class Top {
 public:
  Top() {
    std::cout << "Top created!" << std::endl;
  }

  virtual ~Top() {
    std::cout << "Top destroyed!" << std::endl;
  }

  virtual void say()  {
    std::cout  << "I'm top" << std::endl;
  }
};

class Middle : public Top {
 public:
  Middle() {
    std::cout << "Middle created!" << std::endl;
  }

  virtual ~Middle() {
    std::cout << "Middle destroyed!" << std::endl;
  }

  virtual void say()  {
    std::cout  << "I'm Middle" << std::endl;
  }
};

class Bottom: public Middle{
 public:
  Bottom() {
    std::cout << "Bottom created!" << std::endl;
  }

  virtual ~Bottom() {
    std::cout << "Bottom destroyed!" << std::endl;
  }

  virtual void say()  {
    std::cout  << "I'm Bottom" << std::endl;
  }
};

int main(void) {
  auto bot = Bottom();
  bot.say();
  auto ptr = dynamic_cast<Top*>(&bot);
  ptr->say();
  return 0;
}
