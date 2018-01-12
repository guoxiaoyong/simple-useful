#!/usr/bin/env python3
#coding: utf-8

import smtplib
from email.mime.text import MIMEText
from email.header import Header

sender = 'neodreamer@sina.com'
receiver = 'neodreamer@163.com'
subject = 'python email test'
smtpserver = 'smtp.sina.com'
username = 'neodreamer'
password = '142857'
msg = MIMEText('NiHao','text','utf-8')
msg['Subject'] = Header(subject, 'utf-8')

"""
smtp = smtplib.SMTP()
smtp.connect(smtpserver)
smtp.login(username, password)
smtp.sendmail(sender, receiver, msg.as_string())
smtp.quit()
"""

print msg.as_string();
