#!/usr/bin/python
# -*- coding: utf-8 -*-
import urllib2;
import sys;
import datetime;
from BeautifulSoup import BeautifulSoup

daily_file = sys.argv[1];
history_file = sys.argv[2];
daily_fp = open(daily_file, 'w');
history_fp = open(history_file, 'a');
title_str = "Date,Open Price,High Price,Low Price,Close Price,Turnover(CNY),Volume(share)\n";
daily_fp.write(title_str);
str_today = datetime.date.today().isoformat();
str_line = str_today;
str_url = "http://hq.sinajs.cn/list=sh000300";
req=urllib2.Request(str_url);
resp=urllib2.urlopen(req);
respHtml=resp.read();
HtmlEncoding = "GB2312";
soup = BeautifulSoup(respHtml, fromEncoding=HtmlEncoding);
md_list = soup.contents[0].split(',');
for i in [1, 4, 5, 3, 9, 8]:
    str_line = str_line + ',' + md_list[i];
str_line = str_line + '00\n';
daily_fp.write(str_line);
history_fp.write(str_line);
daily_fp.close();
history_fp.close();
