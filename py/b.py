#!/usr/bin/env python
import imaplib


M = imaplib.IMAP4('imap.sina.com')
M.login('neodreamer', '142857')
M.select()
print M.recent();
print M.list();
typ, data = M.search(None, 'From', 'CSDN')

for num in data[0].split():
    typ, data = M.fetch(num, '(RFC822)')
    print 'Message %s\n%s\n' % (num, data[0][1])

M.logout()


