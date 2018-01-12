import os
import sys

n = 256

for n in range(n):
  print 'a'*4096
  sys.stdout.flush()
  sys.stderr.write('chunk %d\n' %n)
  sys.stderr.flush()
