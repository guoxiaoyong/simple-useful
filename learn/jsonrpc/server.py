import datetime
import sys

import tornado
import tornadorpc
import tornadorpc.json

class Service(tornadorpc.json.JSONRPCHandler):
  def add(self, x, y):
    return x + y

  def ping(self, msg):
    return msg.upper();

  @tornadorpc.async
  def ping_later(self, something):
    ioloop = tornado.ioloop.IOLoop.current()
    ioloop.add_timeout(
        datetime.timedelta(seconds=2),
        self.send_ping_later_response,
        something)

  @tornadorpc.private
  def send_ping_later_response(self, something):
    self.result(something)


def main(argv=None):
  tornadorpc.start_server(Service, port=8080)


if __name__ == "__main__":
  sys.exit(main(argv=sys.argv))
