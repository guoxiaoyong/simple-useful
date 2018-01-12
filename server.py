from tornado.tcpserver import TCPServer
from tornado.iostream import StreamClosedError
from tornado import gen, ioloop
import hexdump
import time
import struct

hello = "\x00\x06\x00\x00\x07\x04\x00\x00\x00\x27"
login = '\x02\x00\x01\x0e\x01\x00\x0cL\x00\x00\x00\x000\x00\x00\x00\x00\x00\x00\x03\x00\xf8\x00\x00\x00\x01\x10\x02\x00\xe019800100\x009999\x00\x00\x00\x00\x00\x00\x00075776\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00ctppassword\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00THOST User\x00FTDC 0\x00\x00\x00\x00\x0000:00:00:00:00:00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x10\x01\x00\x06\x00\x01\x00\x00\x00\x00\x10\x01\x00\x06\x00\x04\x00\x00\x00\x00'
sub = '\x02\x00\x00\xa2\x01\x00\x0cL\x00\x00\x00\x00D\x01\x00\x00\x00\x00\x00\x04\x00\x8c\x00\x00\x00\x00$A\x00\x1fhc1710\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00$A\x00\x1fhc1801\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00$A\x00\x1frb1710\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00$A\x00\x1fag1712\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'
hb = '\x00\x02\x00\x00\x05\x00'
login_ok = '\x02\x00\x00\x35\x01\x03\x0c\x4c\xe4\x30\x01\xe5\x02\xe1\xf5\xe7\x55\xe4\x43\x54\x50\x3a\x4e\x6f\x20\x45\x72\x72\x6f\x72\xef\xef\xef\xef\xe9\x10\x03\xe1\x98\x32\x30\x31\x37\x30\x36\x30\x39\xef\xef\xef\xef\xef\xef\xef\xef\xef\xe9'
sub_ok='\x02\x00\x00\x50\x01\x03\x0c\x4c\xe4\x44\x02\xe5\x05\xe1\xe0\xe5\xe7\x55\xe4\x43\x54\x50\x3a\x4e\x6f\x20\x45\x72\x72\x6f\x72\xef\xef\xef\xef\xe9\x24\x41\xe1\x1f\x68\x63\x31\x37\x31\x30\xef\xea\x24\x41\xe1\x1f\x68\x63\x31\x38\x30\x31\xef\xea\x24\x41\xe1\x1f\x72\x62\x31\x37\x31\x30\xef\xea\x24\x41\xe1\x1f\x61\x67\x31\x37\x31\x32\xef\xea'

md = """
0000   08 00 27 e2 ec ed 52 54 00 12 35 02 08 00 45 00
0010   03 ab 43 2f 00 00 40 06 e0 ab b4 a8 92 bb 0a 00
0020   02 0f 27 1a 93 0a 01 f4 00 8f e5 97 91 6a 50 18
0030   ff ff 48 24 00 00 02 00 03 7f 01 03 0c 4c e4 f1
0040   01 e5 04 05 98 e5 12 01 62 32 30 31 37 30 36 30
0050   39 e1 68 63 31 37 31 30 ef ef ef ef e5 40 a7 64
0060   e5 40 a7 2a e5 40 a7 3c e5 41 22 23 fc e4 40 a7
0070   40 e5 40 a7 74 e5 40 a7 32 e6 02 86 36 41 f2 6c
0080   33 74 c0 e2 41 22 26 44 e4 7f e0 ef ff ff ff ff
0090   ff ff 7f e0 ef ff ff ff ff ff ff 40 a8 8c e5 40
00a0   a5 c6 ed 7f e0 ef ff ff ff ff ff ff 30 39 3a 31
00b0   37 3a 31 31 e5 40 a7 62 e8 3a 40 a7 64 e8 17 7f
00c0   e0 ef ff ff ff ff ff ff e4 7f e0 ef ff ff ff ff
00d0   ff ff e4 7f e0 ef ff ff ff ff ff ff e4 7f e0 ef
00e0   ff ff ff ff ff ff e4 7f e0 ef ff ff ff ff ff ff
00f0   e4 7f e0 ef ff ff ff ff ff ff e4 7f e0 ef ff ff
0100   ff ff ff ff e4 7f e0 ef ff ff ff ff ff ff e4 40
0110   dd 31 65 36 8c 7a fe 32 30 31 37 30 36 30 39 e2
0120   12 01 62 32 30 31 37 30 36 30 39 e1 68 63 31 38
0130   30 31 ef ef ef ef e5 40 a6 b6 e5 40 a6 92 e5 40
0140   a6 8e e5 40 e0 eb e1 40 e4 40 a6 90 e5 40 a6 d0
0150   e5 40 a6 88 e7 10 e0 e4 41 9d f2 1b c0 e3 40 e0
0160   eb 49 c0 e4 7f e0 ef ff ff ff ff ff ff 7f e0 ef
0170   ff ff ff ff ff ff 40 a7 e0 ec e5 40 a5 36 ed 7f
0180   e0 ef ff ff ff ff ff ff 30 39 3a 31 37 3a 30 36
0190   e3 01 f4 40 a6 b4 e8 1a 40 a6 ba e8 03 7f e0 ef
01a0   ff ff ff ff ff ff e4 7f e0 ef ff ff ff ff ff ff
01b0   e4 7f e0 ef ff ff ff ff ff ff e4 7f e0 ef ff ff
01c0   ff ff ff ff e4 7f e0 ef ff ff ff ff ff ff e4 7f
01d0   e0 ef ff ff ff ff ff ff e4 7f e0 ef ff ff ff ff
01e0   ff ff e4 7f e0 ef ff ff ff ff ff ff e4 40 dc 5d
01f0   e0 e1 e0 ec 94 b8 b8 32 30 31 37 30 36 30 39 e2
0200   12 01 62 32 30 31 37 30 36 30 39 e1 72 62 31 37
0210   31 30 ef ef ef ef e5 40 a7 46 e5 40 a7 38 e5 40
0220   a7 30 e5 41 4f 1c 56 e4 40 a7 30 e5 40 a7 54 e5
0230   40 a7 14 e6 1e 84 ae 42 2b b7 55 33 b8 e2 41 4f
0240   21 1a e4 7f e0 ef ff ff ff ff ff ff 7f e0 ef ff
0250   ff ff ff ff ff 40 a8 d8 e5 40 a5 96 ed 7f e0 ef
0260   ff ff ff ff ff ff 30 39 3a 31 37 3a 31 31 e5 40
0270   a7 44 e7 01 6e 40 a7 46 e7 02 95 7f e0 ef ff ff
0280   ff ff ff ff e4 7f e0 ef ff ff ff ff ff ff e4 7f
0290   e0 ef ff ff ff ff ff ff e4 7f e0 ef ff ff ff ff
02a0   ff ff e4 7f e0 ef ff ff ff ff ff ff e4 7f e0 ef
02b0   ff ff ff ff ff ff e4 7f e0 ef ff ff ff ff ff ff
02c0   e4 7f e0 ef ff ff ff ff ff ff e4 40 dd 0f d3 18
02d0   6e 48 c9 32 30 31 37 30 36 30 39 e2 12 01 62 32
02e0   30 31 37 30 36 30 39 e1 61 67 31 37 31 32 ef ef
02f0   ef ef e5 40 b0 15 e5 40 b0 3e e5 40 b0 43 e5 41
0300   1d 36 f0 e4 40 b0 3e e5 40 b0 44 e5 40 b0 10 e6
0310   04 d1 18 42 12 3a 54 96 50 e2 41 1c e0 e1 90 e4
0320   7f e0 ef ff ff ff ff ff ff 7f e0 ef ff ff ff ff
0330   ff ff 40 b1 0d e5 40 ae dc ed 7f e0 ef ff ff ff
0340   ff ff ff 30 39 3a 31 37 3a 31 31 e5 40 b0 14 e8
0350   ae 40 b0 15 e8 3e 7f e0 ef ff ff ff ff ff ff e4
0360   7f e0 ef ff ff ff ff ff ff e4 7f e0 ef ff ff ff
0370   ff ff ff e4 7f e0 ef ff ff ff ff ff ff e4 7f e0
0380   ef ff ff ff ff ff ff e4 7f e0 ef ff ff ff ff ff
0390   ff e4 7f e0 ef ff ff ff ff ff ff e4 7f e0 ef ff
03a0   ff ff ff ff ff e4 40 e0 ee 46 22 e0 e7 f6 80 7f
03b0   32 30 31 37 30 36 30 39 e1
"""
md = hexdump.restore(md)
index = md.find('\x02\x00')
md1 = md[index:]

md = """
0000   08 00 27 e2 ec ed 52 54 00 12 35 02 08 00 45 00
0010   01 2c 43 30 00 00 40 06 e3 29 b4 a8 92 bb 0a 00
0020   02 0f 27 1a 93 0a 01 f4 04 12 e5 97 91 6a 50 18
0030   ff ff 3b 2f 00 00 02 00 00 3a 01 03 0c 4c e4 f1
0040   03 e5 02 e1 55 e4 24 39 e1 35 68 63 31 37 31 30
0050   ef ea 30 39 3a 31 37 3a 31 31 e3 01 f4 32 30 31
0060   37 30 36 30 39 e1 24 34 e1 18 40 a7 62 e8 3b 40
0070   a7 64 e8 17 02 00 00 5f 01 03 0c 4c e4 f1 03 e5
0080   04 e1 81 e4 24 39 e1 35 72 62 31 37 31 30 ef ea
0090   30 39 3a 31 37 3a 31 31 e3 01 f4 32 30 31 37 30
00a0   36 30 39 e1 24 33 e1 1c 40 a7 46 e6 1e 84 be 42
00b0   2b b7 63 bf 78 e2 41 4f 21 21 e4 24 34 e1 18 40
00c0   a7 44 e8 e0 e0 40 a7 46 e7 02 f3 24 81 e1 08 40
00d0   dd 0f d3 1c 74 6d a3 02 00 00 5f 01 03 0c 4c e4
00e0   f1 03 e5 04 e1 81 e4 24 39 e1 35 61 67 31 37 31
00f0   32 ef ea 30 39 3a 31 37 3a 31 31 e3 01 f4 32 30
0100   31 37 30 36 30 39 e1 24 33 e1 1c 40 b0 15 e6 04
0110   d1 1a 42 12 3a 5c 20 28 e2 41 1c e0 e1 90 e4 24
0120   34 e1 18 40 b0 14 e8 af 40 b0 15 e8 3d 24 81 e1
0130   08 40 e0 ee 46 22 db 30 bc be
"""
md = hexdump.restore(md)
index = md.find('\x02\x00')
md2 = md[index:]

print len(login)
print struct.unpack('>H', login[2:4])
print len(md1)
print struct.unpack('>H', md1[2:4])
print len(md2)
print struct.unpack('>H', md2[2:4])
quit()

Gstream = None
send_md = False

def callback(data):
  global Gstream
  global send_md
  print (data,)
  if data[:4] == login[:4]:
    Gstream.write(login_ok)
  elif data[:4] == sub[:4]:
    Gstream.write(sub_ok)
    send_md = True

  if send_md:
    time.sleep(1)
    Gstream.write(md1)
    #Gstream.write(md2)


class MyTcpServer(TCPServer):
  def handle_stream(self, stream, address):
    global Gstream
    Gstream = stream
    try:
      stream.read_bytes(4096, streaming_callback=callback, partial=True)
    except StreamClosedError:
      pass

server = MyTcpServer()
server.listen(8080)
ioloop.IOLoop.current().start()
