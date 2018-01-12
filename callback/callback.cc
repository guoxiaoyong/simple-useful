#include <cassert>

#include "callback.h"

void DoNothing() {}

void ClosureRunner(Closure *closure) { closure->Run(); }

class BarrierClosure : public Closure {
public:
  BarrierClosure(int n, Closure *done) : cb_(done), left_(n) {}

  virtual void CheckIsRepeatable() const {
    // we fail because BarrierClosure can not be called
    // an arbitrary number of times.
    assert(false);
  }

  virtual void Run() {
    assert(left_ >= 0);
    {
      // AutoLock locked(lock_);
      left_--;
      if (left_)
        return;
    }

    Closure *done = cb_;
    delete this;
    done->Run();
  }

private:
  Closure *cb_;
  // Lock lock_;
  int left_;
};

Closure *NewBarrierClosure(int n, Closure *done_closure) {
  assert(n >= 0);
  if (n > 0) {
    return new BarrierClosure(n, done_closure);
  } else {
    // Run supplied closure immediately, without creating any objects.
    done_closure->Run();
    return nullptr;
  }
}
