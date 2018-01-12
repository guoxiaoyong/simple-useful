from tornado import gen
from tornado.concurrent import Future

@gen.coroutine
def fun():
  future = Future()
  future.set_result(123)
  r = yield future
  print r

  #future = Future()
  future.set_result(432)
  r = yield future
  print r

import pdb
pdb.set_trace()
fun()
