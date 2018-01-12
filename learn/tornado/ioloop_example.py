import os
import sys
import time
import datetime
import fcntl
import pdb
import signal

from tornado.ioloop import IOLoop
from tornado.process import Subprocess

def callback():
  if hasattr(callback, 'n'):
    callback.n += 1
  else:
    callback.n = 0

  print "callback"
  if callback.n == 15:
    pass
    IOLoop.current().stop()
    print "stop ioloop"
  else:
    IOLoop.current().call_later(1.01, callback)

subproc = Subprocess(['python', 'subproc.py'],
                     stdin=Subprocess.STREAM,
                     stdout=Subprocess.STREAM,
                     stderr=Subprocess.STREAM)

def handle_line(line):
  #subproc.stdout.read_until('\n', handle_line)
  print datetime.datetime.now()
  sys.stderr.write(line)

subproc.stdout.read_until_close(callback=handle_line, streaming_callback=handle_line)

def handle_ready(*args):
  print args

# pdb.set_trace()
ioloop = tornado.ioloop.IOLoop.current()
ioloop.add_callback(callback)
#ioloop.add_handler(subproc.stdout.fileno(), handle_ready, ioloop.READ)

print datetime.datetime.now()

def handler(signum, frame):
  print os.wait()
  print "subprocess exited!"

signal.signal(signal.SIGCHLD, handler)

ioloop.start()
subproc.wait()
