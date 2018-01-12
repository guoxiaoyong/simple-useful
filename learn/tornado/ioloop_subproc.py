import time
import sys

from tornado.ioloop import IOLoop
from tornado.iostream import PipeIOStream

stdin_stream = PipeIOStream(sys.stdin.fileno())
stdout_stream = PipeIOStream(sys.stdout.fileno())

def read_callback(line):
  stdout_stream.write(line.upper(), write_callback)
  stdin_stream.read_until('\n', read_callback)
  if line.strip() == 'quit':
    print "quit loop"
    IOLoop.current().stop()

def write_callback(*arg):
  print arg

stdin_stream.read_until('\n', read_callback)

IOLoop.current().start()
