import re
import sys
import pdb
import urlparse
import requests 

from SocketServer import StreamRequestHandler
from SocketServer import ThreadingTCPServer
from BaseHTTPServer import BaseHTTPRequestHandler

class ProxyHandler(BaseHTTPRequestHandler):
  def _get_url(self):
    scheme = 'http'
    host = self.headers['host']
    path = self.path
    component = [scheme, host, path, '', '', '']
    return urlparse.urlunparse(component)

  def do_GET(self):
    req = request.get(self._get_url(), self.headers)

  def do_POST(self):
    req = request.post(self._get_url(), self.headers)


address = ('127.0.0.1', 8080)
server = ThreadingTCPServer(address, ProxyHandler)
server.serve_forever()

