#!/usr/bin/env python

import smtplib
import imaplib
import email
import time
import os
import io
import sys

from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from email.mime.image import MIMEImage 
from HTMLParser import HTMLParser

class MLStripper(HTMLParser):
    def __init__(self):
        self.reset()
        #self.strict = False
        #self.onvert_charrefs = True
        self.fed = []

    def handle_data(self, d):
        self.fed.append(d)

    def get_data(self):
        return ' '.join(self.fed)


def strip_tags(html):
    s = MLStripper()
    s.feed(html)
    return s.get_data()


imap_server  = 'imap.sina.com'
smtp_server  = 'smtp.sina.com'
username     = 'mutou9922'
password     = '142857'
from_address = 'mutou9922@sina.com'
to_address = 'neodreamer@163.com'


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
    fp.close()
    return dh[0][0]


def getmailcontent2(msg):
    fp = open(msg, 'r')
    msg = email.message_from_file(fp)

    text = ''
    for par in msg.walk():
        if not par.is_multipart():
            content_type = par.get_content_type()
            if content_type == 'text/html':
                html = par.get_payload(decode=True)
                text = strip_tags(html) 
                text = strip_tags(text) 

    fp.close()
    return text


def getmail():

    try:
        M = imaplib.IMAP4(imap_server)
        r, d = M.login(username, password)
    except:
        return ''

    if r != 'OK':
       print 'failed to login'
       return '' 

    M.select('inbox')
    data = M.recent();

    if not data: 

        print 'no mail in box'
        M.close()
        M.logout()
        return ''

    elif  data[0] == 'OK' and int(data[1][0]) == 0: 

        print 'No mail in box'
        M.close()
        M.logout
        return ''

    else:
        typ, data = M.search(None, 'All');
        num = data[0].split()

        typ, data = M.fetch(num[-1], '(RFC822)')
        msg = data[0][1]
        file_put_contents('mail.eml', msg)
        hdr = getmailcontent('mail.eml')
        msg = getmailcontent2('mail.eml')

        try:
           M.store(num[-1], '+FLAGS', '\\Deleted')
        except:
           M = imaplib.IMAP4(imap_server)
           r, d = M.login(username, password)
           assert r == 'OK', 'login imap server failed'
           M.select('inbox')

        M.expunge()
        M.close()
        M.logout()

        try:
           idx = hdr.index('[GUO]')
           return msg;
        except:
           return ''



def sendmail(content, img=None):

    msgRoot = MIMEMultipart('related')
    t = time.strftime('%Y%m%d_%H_%M_%S')
    msgRoot['Subject'] = '[RESULT] ' + t

    if img is None:
        msgText = MIMEText(content, 'html', 'utf-8')
        msgRoot.attach(msgText) 
    else:
        msgText = MIMEText(content, 'html', 'utf-8')
        msgRoot.attach(msgText) 
 
        fp = open(img, 'rb')
        msgImage = MIMEImage(fp.read())
        fp.close() 
        msgImage.add_header('Content-ID', 'snapshot')
        msgRoot.attach(msgImage) 

    server = smtplib.SMTP(smtp_server)
    server.login(username, password)
    #server.set_debuglevel(1)
    server.sendmail(from_address, to_address, msgRoot.as_string())
    server.quit()


os.chdir('/home/guo/cron')
cmd = 'mplayer -vo jpeg -ao null -frames 1 tv://'
basepath = './'
imgfn = basepath + '00000001.jpg'


if sys.argv[1] == 'mail':

    finword = "system temp file"

    config = file_get_contents("/tmp/block/config")
    n1 = file_get_contents("/tmp/block/n1")
    n2 = file_get_contents("/tmp/block/n2")
    while config != finword: time.sleep(1)

    if len(n1) +  len(n2) > 0:
        cont = "<pre>stdout\n" + n1 + "\n\nstderr\n" + n2 + "</pre>"
        sendmail(cont)
        file_put_contents("/tmp/block/n1", "")
        file_put_contents("/tmp/block/n2", "")

    command = getmail()
    if command is None or len(command) == 0:
         pass
    else:
         #res = os.popen(command).readlines()
         #res = ''.join(res)
         #res = '<p>result:</p>' + '<pre>' + res + '</pre>'
         file_put_contents("/tmp/block/config", command + "\n#end")
         file_put_contents("/tmp/block/config1", command + "\n#end")


if sys.argv[1] == 'grab':
    os.system(cmd)
    if os.path.isfile(imgfn):
        content='<p><b>snapshot</b></p><p><img alt="snapshot" src="cid:snapshot" /></p>'
        sendmail(content, imgfn)
        os.remove(imgfn)

    


