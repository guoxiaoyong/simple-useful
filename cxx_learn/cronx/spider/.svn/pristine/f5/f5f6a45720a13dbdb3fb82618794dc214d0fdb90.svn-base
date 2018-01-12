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
str_url = "http://www.cmegroup.com/trading/agricultural/grain-and-oilseed/soybean_quotes_globex_options.html";
req=urllib2.Request(str_url);
resp=urllib2.urlopen(req);
respHtml=resp.read();
HtmlEncoding = "UTF-8";
soup = BeautifulSoup(respHtml, fromEncoding=HtmlEncoding);

ep="K6";
ul="optionQuotesUnderlierTable1_ZM";
op="optionQuotesProductTable1_OZM";

underlying_future["last"] = ul + ep + '_last';
underlying_future["change"] = ul + ep + '_change';
underlying_future["prior_settle"] = ul + ep + '_priorSettle';
underlying_future["high"] = ul + ep + 'high';
underlying_future["low"] = ul + ep + 'low';
underlying_future["volume"] = ul + ep + 'volume';
underlying_future["hi_low_limit"] = ul + ep + '_highLowLimits';
underlying_future["update_time"] = ul + ep + '_updated';

option["update_time"] = op + ep + '_' + tp + stkpx + '_updated';
option["prior_settle"] = op + ep + '_' + tp + stkpx + '_priorSettle';
option["volume"] = op + ep + '_' + tp + stkpx + '_volume';

mat=10;
striks = soup.findAll('th', {"scope" : "row"});



daily_fp.write(str_line);
history_fp.write(str_line);
daily_fp.close();
history_fp.close();

