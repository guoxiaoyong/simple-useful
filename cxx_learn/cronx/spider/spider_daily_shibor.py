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
title_str = "Date,O/N,1W,2W,1M,3M,6M,9M,1Y\n";
daily_fp.write(title_str);
str_today = datetime.date.today().isoformat();
str_line = str_today;


if len(sys.argv) == 3:
    str_url = "http://www.shibor.org/shibor/Shibor.do";
    req=urllib2.Request(str_url);
    resp=urllib2.urlopen(req);
elif len(sys.argv) == 4:
    resp=open(sys.argv[3]);
else:
    sys.exit(0);

respHtml=resp.read();
HtmlEncoding = "GB2312";
soup = BeautifulSoup(respHtml, fromEncoding=HtmlEncoding);
data_section = soup.find('table', {"class":"shiborquxian"});
ret_all = data_section.findAll('td', align='center');


n=0;
for data_item in ret_all:
    if n%5 == 1:
        str_line = str_line + ',' + str(data_item.contents[0].contents[0].contents[0]);
    if n%5 == 2 or n%5 == 4:
        str_line = str_line + ',' + str(data_item.contents[0]);
    if n%5 == 3:
        if str(data_item.contents[0]).find("downicon.gif") < 0:
            strline = str_line + ',down';
        else:
            strline = str_line + ',up';
    n=n+1;

str_line = str_line + '\n';
daily_fp.write(str_line);
history_fp.write(str_line);
daily_fp.close();
history_fp.close();
