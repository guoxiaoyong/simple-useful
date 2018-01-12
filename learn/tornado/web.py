import tornado.ioloop
import tornado.web
import json


class plain_text(tornado.web.RequestHandler):
  def get(self):
    self.write("You requested the main page")


class story_id(tornado.web.RequestHandler):
  def get(self, story_id):
    self.write("stroy id: %s" % story_id)


class html_page(tornado.web.RequestHandler):
    def get(self):
        self.write('<html><body><form action="/html" method="post">'
                   '<input type="text" name="message">'
                   '<input type="submit" value="Submit">'
                   '</form></body></html>')

    def post(self):
        self.set_header("Content-Type", "text/plain")
        self.write("post message: %s\n" % self.get_argument("message"))


class add(tornado.web.RequestHandler):
    def post(self):
        res = Add(json.loads(self.request.body))
        self.write(json.dumps(res, indent=2))


def Add(input):
    sum = input['num1'] + input['num2']
    result = {}
    result['sum'] = sum
    return result


application = tornado.web.Application([
    (r"/", plain_text),
    (r"/plain", plain_text),
    (r"/story_id/([0-9]+)", story_id),
    (r"/html", html_page),
    (r"/add", add),
])


if __name__ == "__main__":
    application.listen(8888)
    tornado.ioloop.IOLoop.current().start()
