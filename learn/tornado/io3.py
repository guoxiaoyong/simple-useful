import time
import subprocess

subproc = subprocess.Popen(['python', 'io2_subproc.py'], stdout=subprocess.PIPE)
time.sleep(10)
print """
This tests show that the Pipe has a buffer that is approximatedly 60K.
If the buffer is full, then the write will be blocked.
"""

print len(subproc.stdout.read(4096*14))
time.sleep(10)
