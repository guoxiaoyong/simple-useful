#!/usr/bin/env python3
#coding: utf-8
import smtplib
from email.mime.text import MIMEText
from email.header import Header
sender = '***'
receiver = '***'
subject = 'python email test'
smtpserver = 'smtp.163.com'
username = '***'
password = '***' 

msg = MIMEText('���','text','utf-8')#�����������utf-8'�����ֽ��ַ�����Ҫ
msg['Subject'] = Header(subject, 'utf-8') 

smtp = smtplib.SMTP()
smtp.connect('smtp.163.com')
smtp.ehlo()
smtp.starttls()
smtp.ehlo()
smtp.set_debuglevel(1)
smtp.login(username, password)
smtp.sendmail(sender, receiver, msg.as_string())
smtp.quit()