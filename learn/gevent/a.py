import gevent
import sys
from gevent.select import select

def readcmd():
  for n in range(5):
    sys.stdout.write('-->')
    sys.stdout.flush()
    select([sys.stdin],[],[])
    cmd = raw_input()
    print n, 'readcmd'
    print cmd.upper()

def readpipe(fd):
  for n in range(5):
    select([fd],[],[])
    cmd = fd.readline()
    print n, 'readpipe'
    print cmd

fd = open('/tmp/pipe', 'r')

g1 = gevent.spawn(readcmd)
g2 = gevent.spawn(readpipe, fd)
gevent.joinall([g1, g2])
