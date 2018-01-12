from tornado import ioloop
from tornado import gen
from tornado import concurrent

Future = concurrent.Future

import time

ping = Future()
pong = Future()
n = 0

@gen.coroutine
def consumer():
  global ping
  global pong
  while True:
    num = yield ping
    print "cosume %d" % n
    pong.set_result(True)
    ping = Future()
    if num == 10:
      ioloop.IOLoop.current().stop()

@gen.coroutine
def producer():
  global ping
  global pong
  global n
  while True:
    print "produce"
    ping.set_result(n)
    flag = yield pong
    pong = Future()
    n += 1

#ioloop.IOLoop.current().add_callback(consumer)
#ioloop.IOLoop.current().add_callback(producer)

print consumer()
print producer()

ioloop.IOLoop.current().start()
