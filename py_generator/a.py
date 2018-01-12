class Box(object):
  def __init__(self):
    self._num = 0

  @property
  def num(self):
    print "get num =", self._num
    return self._num

  @num.setter
  def num(self, value):
    print "set num =", value
    self._num = value


def fun():
  value = yield 1
  for _ in range(10):
    value = yield value*value


gen = fun()
first = gen.send(None)
print first
x = first
x = gen.send(x+1); print x
x = gen.send(x+1); print x
x = gen.send(x+1); print x
x = gen.send(x+1); print x
