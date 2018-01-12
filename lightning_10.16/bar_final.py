from threading import Thread
import imp

"""
class Bar(Thread):
    def run(self):
        print "Bar! import lock is already held:" + str(imp.lock_held())
        import urllib
        print "My name is Bar!"

bar = Bar()
bar.start()
bar.join()
"""

print "Bar! import lock is already held:" + str(imp.lock_held())
import urllib
print "My name is Bar!"
