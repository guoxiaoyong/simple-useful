#!/usr/bin/python
# -*- coding: utf-8 -*-
import urllib2;
import re;
import string;
import sys;
from BeautifulSoup import BeautifulSoup
month_num = {
  'Jan' : '01',
  'Feb' : '02',
  'Mar' : '03',
  'Apr' : '04',
  'May' : '05',
  'Jun' : '06',
  'Jul' : '07',
  'Aug' : '08',
  'Sep' : '09',
  'Oct' : '10',
  'Nov' : '11',
  'Dec' : '12'
  ''
};

def process_date(raw_date):
    global month_num;
    raw_list=raw_date.split(' ');
    month_str=month_num[raw_list[0]];
    day_list=raw_list[1].split(',');
    if len(day_list[0]) == 1:
        day_str = '0' + day_list[0];
    else:
        day_str = day_list[0];
    year_str = raw_list[2];
    return year_str + '-' + month_str + '-' + day_str;

def process_num(raw_num):
    raw_list=raw_num.split(',');
    sz = len(raw_list);
    str_num=raw_list[0];
    for i in range(1,sz):
        str_num = str_num+raw_list[i];
    return str_num;

str_url = "http://finance.yahoo.com/q/hp?s=%5EKS11+Historical+Prices";
req=urllib2.Request(str_url);
resp=urllib2.urlopen(req);
respHtml=resp.read();
HtmlEncoding = "UTF-8";
soup = BeautifulSoup(respHtml, fromEncoding=HtmlEncoding);
tag_top = soup.find('table', {"class":"yfnc_datamodoutline1"});
tag_body = tag_top.contents[0].contents[0].contents[0];
str_date = process_date(tag_body.contents[1].contents[0].contents[0]);
open_price = process_num(tag_body.contents[1].contents[1].contents[0]); 
high_price = process_num(tag_body.contents[1].contents[2].contents[0]);
low_price = process_num(tag_body.contents[1].contents[3].contents[0]);
close_price = process_num(tag_body.contents[1].contents[4].contents[0]);
volume = process_num(tag_body.contents[1].contents[5].contents[0]);
if volume != "0":
    daily_file = sys.argv[1];
    history_file = sys.argv[2];
    daily_fp = open(daily_file, 'w');
    history_fp = open(history_file, 'a');
    title_str = "Date,Open Price,High Price,Low Price,Close Price,Volume(KRW)\n";
    daily_fp.write(title_str);
    day_market_data =  str_date+","+open_price+","+high_price+","+low_price+","+close_price+","+volume+'\n';
    daily_fp.write(day_market_data);
    history_fp.write(day_market_data);
    daily_fp.close();
    history_fp.close();
   
        
    

