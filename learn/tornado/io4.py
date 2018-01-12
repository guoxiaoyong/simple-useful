import os
import sys
import time
import datetime
import fcntl
import pdb
import signal

from tornado.ioloop import IOLoop
from tornado.process import Subprocess

def handler(signum, frame):
  print "subproc return value:", os.wait()
  print "subprocess exited!"
  ioloop.current().stop()

signal.signal(signal.SIGCHLD, handler)

subproc = Subprocess(['python', 'io4_subproc.py'],
                     stdin=Subprocess.STREAM,
                     stdout=Subprocess.STREAM,
                     stderr=Subprocess.STREAM)

class LineHandler(object):
  line_buffer = ''

  @classmethod
  def handle(cls, line):
    cls.line_buffer += line
    lines = cls.line_buffer.split('\n')
    for ln in lines[:-1]:
      sys.stderr.write(ln.upper())
      sys.stderr.write('\n')
    cls.line_buffer = lines[-1]

def handle_line(line):
  if hasattr(handle_line, 'line_buffer'):
    handle_line.line_buffer = line
  else:
    handle_line.line_buffer += line
  line_buffer += line
  lines = line_buffer.split('\n')
  for ln in lines[:-1]:
    sys.stderr.write(ln.upper())
    sys.stderr.write('\n')
  line_buffer = lines[-1]

subproc.stdout.read_until_close(streaming_callback=LineHandler.handle)

print datetime.datetime.now()
ioloop = IOLoop.current()
ioloop.start()
