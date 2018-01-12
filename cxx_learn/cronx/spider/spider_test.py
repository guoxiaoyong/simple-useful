#!/usr/bin/python
# -*- coding: utf-8 -*-
import urllib2;
import re;
from BeautifulSoup import BeautifulSoup;
#str_url="http://www.shibor.org/shibor/Shibor.do?date=2013-08-09";
str_url="http://hq.sinajs.cn/list=sh000300";
req=urllib2.Request(str_url);
resp=urllib2.urlopen(req);
respHtml=resp.read();
HtmlEncoding = "GB2312";
soup = BeautifulSoup(respHtml, fromEncoding=HtmlEncoding);
md_str =  soup.contents[0];
md_list = md_str.split(',');
print md_list[1];
#data_section = soup.find('table', {"class":"shiborquxian"});
#print data_section.contents[1].find('td', align='center').contents[0]
#print data_section
"""
data_section = soup.find('table', {"class":"shiborquxian"});
ret_all = data_section.findAll('td', align='center');
for data_item in ret_all:
    print data_item.contents[0]

        ret_all = soup.findAll('tr');
        for day_item in ret_all:
            sub_search = day_item.findAll(target="_blank");
            if(len(sub_search) > 0):
                str_day = day_item.contents[1].contents[0].contents[1].contents[0].strip();
                open_price = day_item.contents[3].contents[0].contents[0];
                high_price = day_item.contents[5].contents[0].contents[0];
                close_price = day_item.contents[7].contents[0].contents[0];
                low_price = day_item.contents[9].contents[0].contents[0];
                volume = day_item.contents[11].contents[0].contents[0];
                turnover = day_item.contents[13].contents[0].contents[0];
                day_market = str_day+","+open_price+","+high_price+","+close_price+","+low_price+","+volume+","+turnover+"\n";
                md_list.append(day_market);
                #print day_market
        #print md_list
        i=len(md_list)-1
        if i<0:
            print "Did not get market data for year ",year," season ",jidu;
            continue;
        #print "i=",i
        while i>=0:
            md_fp.write(md_list[i]);
            i=i-1;
        print "Finish processing year ",year," season ",jidu;
        md_list=[];
md_fp.close()
"""
