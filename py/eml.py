#!/usr/bin/env python

import email

#fp = open("eml.eml", "r")
#msg = email.message_from_file(fp)
msg = email.message_from_file("eml.eml")
subject = msg.get('subject')
print type(subject)
h = email.Header.Header(subject)
dh = email.Header.decode_header(h)

for par in msg.walk():
    if not par.is_multipart():
        print par.get_payload(decode=True)



#fp.close()
