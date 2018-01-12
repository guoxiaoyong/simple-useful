from tornado import ioloop
from tornado import gen
from tornado import concurrent
import time


def make_future(fun):
  for n in fun():
    yield Future()


@gen.coroutine
def consumer():
  future = cocurrent.Future()
  while True:
    num = yield ping
    print "cosume %d" % n
    pong.set_result(True)
    ping = Future()
    if num == 10:
      ioloop.IOLoop.current().stop()


ioloop.IOLoop.current().start()
