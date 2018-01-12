from parsec import (
    Entity,
    DriverEntity
)


class Sieve(Entity):
  def init(self, sieve_id):
    self._sieve_id = sieve_id

  @property
  def sieve_id(self):
    return self._sieve_id

  def run(self):
    myprime = yield self.receive()

    print "Sieve number %d is for prime number %d"  % (self.sieve_id, myprime)
    next_sieve = Sieve(self.sieve_id + 1)
    while True:
      number = yield self.receive()
      if number % myprime:
        self.send(next_sieve, number)

  def finalize(self):
    print "%d terminated!" % id(self)


import sys
class Driver(DriverEntity):
  def run(self):
    first = Sieve(2)
    for number in range(2, int(sys.argv[1])):
      self.send(first, number)

Driver()
