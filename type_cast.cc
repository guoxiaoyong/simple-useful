#include <iostream>

enum class State {
  SUBMITTED,
  WORKING,
  DEAD
};

enum class CancelState {
  SUBMITTED,
  ACCEPTED,
};

struct StateWrapper {
  State state;
  CancelState cancel_state;

  StateWrapper& operator=(State s) {
    std::cout << "State" << static_cast<int>(s) << std::endl;
    state = s;
    return *this;
  }

  StateWrapper& operator=(CancelState s) {
    std::cout << "CancelState" << static_cast<int>(s) << std::endl;
    cancel_state = s;
    return *this;
  }
};



int main(void) {
  StateWrapper state;
  state = State::DEAD;
  state = State::WORKING;
  state = State::SUBMITTED;
  state = CancelState::ACCEPTED;
  return 0;
}
