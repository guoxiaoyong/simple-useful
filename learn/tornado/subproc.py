import time
import sys

def print_message(msg):
  sys.stdout.write(msg)
  sys.stdout.flush()

n = 10
for _ in range(n):
  print_message('hello\n')
  time.sleep(1)

#print_message('\n')
