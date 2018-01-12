import greenlet

def test1():
  print(12)
  gr2.switch()
  print(34)
  print('test1', greenlet.getcurrent())
  gr2.switch()


def test2():
  print('test2', greenlet.getcurrent())
  print(56)
  gr1.switch()
  print(78)


print('main', greenlet.getcurrent())
gr1 = greenlet.greenlet(test1)
gr2 = greenlet.greenlet(test2)
print('gr1:', gr1)
print('gr2:', gr2)
gr1.switch()
