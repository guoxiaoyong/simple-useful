from microbit import *
import music
import random

def wait_for_button_press(sleep_time=1000):
  na = button_a.get_presses()
  nb = button_b.get_presses()
  while True:
    if na < button_a.get_presses():
        return 'a'
    if nb < button_b.get_presses():
        return 'b'
    sleep(sleep_time)

def gen_random_equation():
    while True:
      a = random.choice(range(1,10))
      b = random.choice(range(1,10))
      e = random.choice([-2, -1, 1, 2, 0, 0, 0, 0])
      op = random.choice('+-x')
      if op == '-' and a < b:
          continue
      else:
          break
    expr = "%s%s%s" % (a, op, b)
    result = eval(expr.replace('x', '*')) + e
    return e == 0, "%s%s%s=%s" % (a, op, b, result)

while True:
    result, eq = gen_random_equation()
    display.scroll(eq)
    button = wait_for_button_press()
    if ((button == 'a' and result) or
        (button == 'b' and not result)):
        display.show(Image.YES)
    else:
        display.show(Image.NO)
    sleep(2000)
