class PuzzleBlock(object):
  def __init__(self, content):
    assert len(content) == 9, content
    self.content = content

  def get(self, x, y):
    return self.content[x*3+y]


class Puzzle(object):
  def __init__(self, content):
    assert len(content) == 81
    self.content = content

  @staticmethod
  def index_1d_to_2d(n):
    x = n // 9
    y = n % 9
    return (x, y)

  @staticmethod
  def index_2d_to_block_2d(x, y):
    return (x//3, y//3)

  @staticmethod
  def index_1d_to_block_2d(n):
    x = n // 9
    y = n % 9
    return (x//3, y//3)

  def get_line(self, n):
    assert 0 <= n and n <= 9
    start = n*9
    end = start + 9
    return self.content[start:end]

  def get_row(self, n):
    assert 0 <= n and n <= 9
    return self.content[n:end:9]

  def get_block(self, x, y):
    res = []
    for n in range(81):
      bx, by = self.index_1d_to_block_2d(n)
      if bx == x and by == y:
        res.append(content[n])
    return PuzzleBlock(''.join(res))

res = """
<!DOCTYPE html>
<html lang="en">
<style type="text/css">
body {
  font-size: 20pt;
  padding: 2cm;
}

.outtbl {
  border-collapse:collapse;
  border: solid 2px;
}

.intbl {
  border-collapse:collapse;
  border: solid 2px;
}

table td{
  border:1px solid;
}

.intd {
  padding: 5mm;
}

</style>
<body>
"""

lines = open('a.txt').readlines()
for line in lines:
  content = line.strip()[:-1]
  if len(content) != 81: continue
  print '<table class="outtbl">'
  puz = Puzzle(content)
  for X in range(3):
    print '<tr>'
    for Y in range(3):
      print '<td class="outtd"><table class="intbl">'
      puzblock = puz.get_block(X, Y)
      for x in range(3):
        print '<tr>'
        for y in range(3):
          num = puzblock.get(x, y)
          if num != '.':
            print '<td class="intd">%s</td>' % num
          else:
            print '<td class="intd"><span style="visibility:hidden">0</span></td>'
        print '</tr>'
      print '</table></td>'
    print '</tr>'
  print '</table>'
  print '<hr />'

print """
</body>
<html>
"""

print(res)
