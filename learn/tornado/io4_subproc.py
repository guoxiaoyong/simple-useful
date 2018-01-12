import os
import sys
import time

num_iters = 3
line = 'The quick brown fox jumps over the lazy dog'

for n in range(num_iters):
  for k in line:
    sys.stdout.write(k)
    sys.stdout.flush()
    #time.sleep(0.1)
  sys.stdout.write('\n')
  sys.stdout.flush()
  sys.stderr.write('chunk %d\n' %n)
  sys.stderr.flush()
