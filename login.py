# -*- coding:utf-8 -*-
# author:Simon
# updatetime:2016年3月17日 17:35:35
# 功能：爬虫之模拟登录，urllib和requests都用了...

import urllib
import urllib2
import requests
import re

headers = {'User-Agent':'Mozilla/5.0 (Windows NT 6.2) AppleWebKit/535.11 (KHTML, like Gecko) Chrome/17.0.963.12 Safari/535.11'}

def get_xsrf():
    firstURL = "http://www.zhihu.com/#signin"
    request = urllib2.Request(firstURL,headers = headers)
    response = urllib2.urlopen(request)
    content = response.read()
    pattern = re.compile(r'name="_xsrf" value="(.*?)"/>',re.S)
    _xsrf = re.findall(pattern,content)
    return _xsrf[0]

def login(par1):
    s = requests.session()
    afterURL = "https://www.zhihu.com/explore"        # 想要爬取的登录后的页面
    loginURL = "http://www.zhihu.com/login/email"     # POST发送到的网址
    login = s.post(loginURL, data = par1, headers = headers)                  # 发送登录信息，返回响应信息（包含cookie）
    response = s.get(afterURL, cookies = login.cookies, headers = headers)    # 获得登陆后的响应信息，使用之前的cookie
    return response.content

xsrf = get_xsrf()
print "_xsrf的值是：" + xsrf
data = {"email":"xxx","password":"xxx","_xsrf":xsrf}
print login(data)
