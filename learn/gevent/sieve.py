import gevent
from gevent.queue import JoinableQueue


class Sieve(gevent.Greenlet):
  def __init__(self, myid):
    super(Entity, self).__init__()
    self.id = myid
    self.queue = JoinableQueue()

  def _run(self):
    myprime = self.queue.get()
    print(myprime)
    next_sieve = Sieve()
    while True:
      number = self.queue.get()
      if number % myprime:
        self.send(next_sieve, number)
