from threading import Thread
import imp

class Foo(Thread):
    def run(self):
        print "Foo! import lock is already held:" + str(imp.lock_held())
        import bar_final
        print "My name is Foo!"

foo = Foo()
foo.start()
foo.join()
