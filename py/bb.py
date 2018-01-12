#!/usr/bin/env python
import imaplib
import email


M = imaplib.IMAP4('imap.sina.com')
M.login('neodreamer', '142857')
M.select()
data = M.recent();

if not data:
    quit();

if data[0] == 'OK' and int(data[1][0]) == 0:
    quit()


typ, data = M.search(None, 'Subject', '[TEST]');
num = data[0].split()
typ, data = M.fetch(num[-1], '(RFC822)')
msg = data[0][1]

#eparser = email.parser.Parser();
print msg
M.logout()


