from pcapng import FileScanner
from pcapng.blocks import EnhancedPacket
from scapy.layers.l2 import Ether
#from scapy.layers.inet import IP, TCPInterfaceStatistics

filename = "ctp_md_packet.pcapng"
with open(filename) as fp:
  scanner = FileScanner(fp)
  for block in scanner:
    if isinstance(block, EnhancedPacket):
      assert block.interface.link_type == 1
      #decoded = Ether(block.packet_data)
      #print dir(decoded)
