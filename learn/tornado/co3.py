import sys

from tornado.ioloop import IOLoop
from tornado.gen import coroutine
from tornado.concurrent import Future

@coroutine
def asyn_sum(a, b):
    future = Future()
    print "begin calculate:sum %d+%d" % (a,b), sys.getrefcount(future)

    def callback(a, b):
        print("calculating the sum of %d+%d:" % (a,b))
        print "callbacks of future:", future._callbacks, sys.getrefcount(future)
        future.set_result(a+b)
        print "after set result", sys.getrefcount(future)
    IOLoop.current().add_callback(callback, a, b)

    print("before yield")
    print "callbacks of future:", future._callbacks
    result = yield future
    print "after yielded", sys.getrefcount(future)
    print("the sume of %d+%d is %d" % (a, b, result))
    IOLoop.current().stop()

def main():
    asyn_sum(2,3)
    IOLoop.current().start()

if __name__ == "__main__":
    main()
