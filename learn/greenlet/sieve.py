import greenlet

def sieve(nums):
  prime = nums[0]
  print(prime)
  next_sieve = greenlet.greenlet(sieve)
  next_nums = [num for num in nums if num % prime]
  if next_nums:
    next_sieve.switch(next_nums)


class Entity(object):
  def __init__(self):
    self.greenlet = greenlet.greenlet(self.run)

  def on_message(self, message):
    self.greenlet.switch(message)

  def run(self):
    pass


class Driver(object):
  def __init__(self):
    self.messages = {}

  def schedule(self):
    while True:
      for entity, messages in self.messages.items():
        if messages:
          entity.on_message(messages.pop(0))


class MyDriver(Driver):
  def run(self):
    sieve = Sieve(2)
    self.messages[sieve] = list(range(3, 10))
    while True:
    messages[sieve_let] = list(range(3, 10)) + [None]
    op = sieve_let.switch(2)
    while True:
      if op[0] == 'receive':
        messages[op[1]]
        op = op[1].switch(messages[op[1]][0])
        messages[sieve_let].pop(0)
      elif op[0] == 'send':
        messages[op[1]] = messages.get(op[1], [])
        messages[op[1]].append(op[2])
        op = ['receive', op[1]]

driver = greenlet.greenlet(sieve)
driver.switch(list(range(2,100)))
