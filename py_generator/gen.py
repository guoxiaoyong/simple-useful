def gen(x):
  if x <= 10:
    yield x
    for v in gen(x + 1):
      yield v
  print x

def dec(gen):
  def new_gen(x):
    g = gen(x)
    value = g.next()
    for v in g:
      yield value
      value = v
  return new_gen


some_gen = dec(gen)
for n in some_gen(5):
  pass
