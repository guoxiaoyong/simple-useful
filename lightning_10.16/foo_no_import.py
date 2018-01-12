from threading import Thread

class Foo(Thread):
    def run(self):
        import bar_no_import
        print "My name is Foo!"

foo = Foo()
foo.start()
foo.join()
