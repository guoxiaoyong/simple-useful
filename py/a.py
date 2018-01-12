#!/usr/bin/env python
import poplib

M = poplib.POP3('pop.sina.com')
M.user('neodreamer');
M.pass_('142857');
print M.getwelcome();
print M.stat();
print M.list();
print M.retr(1)
M.quit();
