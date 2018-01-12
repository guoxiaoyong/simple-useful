import os
import sys
import json
import readline
import Queue
import threading

def foo(a, b):
  return a+b

def bar():
  print "Module pytest is imported!"
  print sys.version

while True:
  cmd = raw_input('--> ')
  if cmd == 'quit':
    break
  else:
    print cmd.upper()

t = threading.Thread(target=bar)
t.start()
t.join()
