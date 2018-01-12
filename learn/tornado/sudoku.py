from parsec import (
    Singleton,
    Entity,
    DriverEntity
)


def pretty_print(s):
  res = ''
  for idx, ch in enumerate(s):
    m, n = idx // 9, idx % 9
    if n == 0 and (m == 3 or m == 6):
      res += '-------|-------|-------\n'
    res += " %s" % ch
    if n == 2 or n == 5:
      res += " |"
    elif n == 8:
      res += '\n'
  print res


@Singleton
class NodeManager(object):
  def __init__(self):
    self.nodes = [None]*81
    self.p = pretty_print

  def __del__(self):
    self.p(''.join(str(node.value) for node in self.nodes))

  def add(self, idx, node):
    self.nodes[idx] = node

  def get_connected(self, m, n):
    connected = []
    for idx, node in enumerate(self.nodes):
      mi, ni = idx // 9, idx % 9
      if mi == m and ni == n:
        continue
      elif mi == m or ni == n:
        connected.append(node)
      else:
        block_m, block_n = m // 3, n // 3
        block_mi, block_ni = mi // 3, ni // 3
        if block_m == block_mi and block_n == block_ni:
          connected.append(node)
    return connected


class Node(Entity):
  def init(self, value, m, n):
    assert 0 <= value <= 9
    self.pos = (m, n)
    self._value = set(range(1, 10)) if value == 0 else set([value])

  @property
  def value(self):
    if len(self._value) == 1:
      return list(self._value)[0]
    else:
      return 0

  def run(self):
    # First message is ready signal.
    yield self.receive()

    connected_nodes = NodeManager.get_instance().get_connected(*self.pos)
    if self.pos == (0, 2):
      print self._value
    for _ in range(1):
      for entity in connected_nodes:
        self.send(entity, set(range(1, 10)) - self._value)
      numset = yield self.receive()
      if self.pos == (0, 2):
        print numset
      self._value &= numset
      if self.pos == (0, 2):
        print self._value


class Driver(DriverEntity):
  def run(self):
    puz = '..5...19........68....6..7..8.....3..........6..74...14..18...7.9...38..15.9.6.4.'
    puz = puz.replace('.', '0')
    pretty_print(puz)
    for idx, value in enumerate(puz):
      m, n = idx // 9, idx % 9
      node = Node(int(value), m, n)
      NodeManager.get_instance().add(idx, node)
      self.send(node, True)

Driver()
