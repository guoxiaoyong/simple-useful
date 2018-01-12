from threading import Thread
import imp

class Bar(Thread):
    def run(self):
        import urllib
        print "My name is Bar!"

bar = Bar()
bar.start()
bar.join()
