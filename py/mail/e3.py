#!/usr/bin/env python3
#coding: utf-8
import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from email.mime.image import MIMEImage 
sender = 'neodreamer@sina.com'
receiver = 'neodreamer@163.com'
subject = 'python email test'
smtpserver = 'smtp.sina.com'
username = 'neodreamer'
password = '142857' 
msgRoot = MIMEMultipart('related')
msgRoot['Subject'] = 'test message' 
msgText = MIMEText('<b>Some <i>HTML</i> text</b> and an image. <img alt="" src="cid:image1" /> good!','html','utf-8')
msgRoot.attach(msgText) 

fp = open('volcone.png', 'rb')
msgImage = MIMEImage(fp.read())
fp.close() 
msgImage.add_header('Content-ID', 'image1')
msgRoot.attach(msgImage) 

smtp = smtplib.SMTP()
smtp.set_debuglevel(1)
smtp.connect(smtpserver)
smtp.login(username, password)
smtp.sendmail(sender, receiver, msgRoot.as_string())
smtp.quit()
#print msgRoot.as_string()
