from threading import Thread

class Bar(Thread):
    def run(self):
        print "My name is Bar!"

bar = Bar()
bar.start()
bar.join()
