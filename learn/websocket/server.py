import tornado.ioloop
import tornado.web
import tornado.websocket

class MainHandler(tornado.web.RequestHandler):
  def get(self):
    self.write('hello world')


class EchoWebSocket(tornado.websocket.WebSocketHandler):
  def open(self):
    print("WebSocket opened")

  def on_message(self, message):
    self.write_message(u"You said: " + message)

  def on_close(self):
    print("WebSocket closed")


routes = [
    (r'/', MainHandler),
    (r'/ws', EchoWebSocket),
]

app = tornado.web.Application(routes)
app.listen(9898)
tornado.ioloop.IOLoop.current().start()
