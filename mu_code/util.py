# Copyright (c) 2017, Xiaoyong Guo
# Author: xguo

from microbit import *

def wait_for_button_press(sleep_time=1000):
  na = button_a.get_presses()
  nb = button_b.get_presses()
  while True:
    if na < button_a.get_presses():
        return 'a'
    if nb < button_b.get_presses():
        return 'b'
    sleep(sleep_time)
