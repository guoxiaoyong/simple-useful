def RetFun():
  x = [10, 12]
  print id(x)
  def fun():
    print x
    print id(x)
    x.append(123)
  return fun


a = RetFun()
b = RetFun()
a()
b()
