#!/usr/bin/env python

import smtplib
import imaplib
import email
import time
import os
import io

imap_server  = 'imap.sina.com'
smtp_server  = 'smtp.sina.com'
username     = 'neodreamer'
password     = '142857'
from_address = 'neodreamer@sina.com'
#to_address   = 'nz.1984@hotmail.com'
#to_address   = 'neodreamerus@hotmail.com'
to_address   = 'neodreamer@163.com'


def file_put_contents(filename, content):
    f = open(filename, 'w')
    f.write(content);
    f.close();


def file_get_contents(filename):
    f = open(filename, 'r');
    s = f.read()
    f.close()
    return s
    

def getmailcontent(msg):
    fp = open(msg, 'r')
    msg = email.message_from_file(fp)
    subject = msg.get('subject')
    h = email.Header.Header(subject)
    dh = email.Header.decode_header(h)
    return dh


def getmail():
    M = imaplib.IMAP4(imap_server)
    M.login(username, password)
    M.select()
    data = M.recent();

    if not data:
        quit();

    if data[0] == 'OK' and int(data[1][0]) == 0:
        quit()

    typ, data = M.search(None, 'All');
    print data
    num = data[0].split()
    print num
    typ, data = M.fetch(num[-1], '(RFC822)')
    msg = data[0][1]
    file_put_contents('mail.eml', msg);
    msg = getmailcontent('mail.eml')
    M.logout()

    msg = msg[0][0]

    try:
       idx = msg.index('[TEST]')
       return msg[idx+6:]
    except:
       return ''



def sendmail(content, img=None):

    msgRoot = MIMEMultipart('related')
    t = time.strftime('%Y%m%d_%H_%M_%S')
    msgRoot['Subject'] = '[MQT RESULT] ' + t

    if img is None:
        msgText = MIMEText('<pre>'+content+'</pre>', 'html', 'utf-8')
        msgRoot.attach(msgText) 
    else:
        msgText = MIMEText('<pre>' + content + '</pre>' + '<img alt="" src="cid:image1" />', 'html', 'utf-8')
        msgRoot.attach(msgText) 
 
        fp = open(img, 'rb')
        msgImage = MIMEImage(fp.read())
        fp.close() 
        msgImage.add_header('Content-ID', 'image1')
        msgRoot.attach(msgImage) 


    server = smtplib.SMTP(smtp_server)
    server.login(username, password)
    server.set_debuglevel(1)
    server.sendmail(from_address, to_address, msg)
    server.quit()




outfn = 'content.txt'
infn  = 'result.txt'
imgfn = 'image.png'

last_checkmail_time = time.clock()

while True:

    now_time = time.clock()
    if now_time - last_checkmail_time > 10:
        last_checkmail_time = now_time
        command = getmail()
        print command
        if len(command) >  10:
            command += '\n%done'
            file_put_contents(outfn, command)

    if os.path.isfile(infn):

        suffix = time.strftime('%Y%m%d_%H_%M_%S')
        content = file_get_contents(infn)

        if os.path.isfile(imgfn):
            sendmail(content, imgfn)
            os.rename(imgfn, imgfn + suffix)
        else:
            sendmail(content)

        new_infn = 'content_' + suffix + '.txt'
        file_put_contents(new_infn, content)
        os.remove(infn)

    time.sleep(1) 

    

