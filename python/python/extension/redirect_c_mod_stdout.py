import sys
import StringIO

sio = StringIO.StringIO()
stdout = sys.stdout
import simple
sys.stdout = sio
b = simple.add(1,2)
#sys.stdout = stdoutprint b
