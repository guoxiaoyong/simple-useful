from threading import Thread
import imp
import pdb

class Foo(Thread):
    def run(self):
        import bar_no_comment
        print "My name is Foo!"

foo = Foo()
foo.start()
foo.join()
