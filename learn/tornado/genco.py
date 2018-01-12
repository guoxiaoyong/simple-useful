from tornado import gen

@gen.coroutine
def fun():
  yield 12
  yield 13

r = fun()
print r.exception()
