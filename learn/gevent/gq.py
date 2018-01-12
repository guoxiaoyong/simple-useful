import gevent
from gevent.queue import JoinableQueue

def do_work(item):
  print(item)

def worker():
  while True:
      item = q.get()
      try:
          do_work(item)
      finally:
          q.task_done()

q = JoinableQueue()
for i in range(10):
     gevent.spawn(worker)

for item in range(10):
    q.put(item)

q.join()  # block until all tasks are done
