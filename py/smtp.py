#!/usr/bin/env python
import smtplib

fromaddr = 'neodreamer@sina.com'
toaddrs  = 'neodreamer@163.com'
msg = ("From: %s\r\nTo: %s\r\nSubject: %s\r\n\r\n" % (fromaddr, toaddrs, "test mail"))
msg += "this is a test email send by python"
print msg

server = smtplib.SMTP('smtp.sina.com')
server.login('neodreamer', '142857')
server.set_debuglevel(1)
server.sendmail(fromaddr, toaddrs, msg)
server.quit()
