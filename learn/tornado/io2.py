import time
from tornado.process import Subprocess

subproc = Subprocess(['python', 'io2_subproc.py'],
                     stdout=Subprocess.STREAM)
time.sleep(10)
print """
This tests show that the PipeIOStream has a buffer that is approximatedly 60K.
If the buffer is full, then the write will be blocked.
"""
