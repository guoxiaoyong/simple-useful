from tornado import concurrent

def fun1(f):
  print "fun1: ", f.result()

def fun2(f):
  print "fun2: ", f.result()

future = concurrent.Future()

try:
  future.result()
except Exception as e:
  print e

future.add_done_callback(fun1)
future.add_done_callback(fun2)

future.set_result(123)
