#!/usr/bin/env python3
#coding: utf-8
import smtplib
from email.mime.text import MIMEText 

sender = '***'
receiver = ['***','****',����]
subject = 'python email test'
smtpserver = 'smtp.163.com'
username = '***'
password = '***' 

msg = MIMEText('���','text','utf-8') 

msg['Subject'] = subject 

smtp = smtplib.SMTP()
smtp.connect('smtp.163.com')
smtp.login(username, password)
smtp.sendmail(sender, receiver, msg.as_string())
smtp.quit()