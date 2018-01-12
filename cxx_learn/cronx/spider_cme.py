#!/usr/bin/python
# -*- coding: utf-8 -*-
import urllib2;
import sys;
import datetime;
import csv;
import base64;
import HTMLParser;
from BeautifulSoup import BeautifulSoup

html_parser = HTMLParser.HTMLParser();

MaturityDates = {
'201603':'H6',
'201604':'J6',
'201605':'K6',
'201607':'N6',
'201608':'Q6',
'201609':'U6',
'201610':'V6',
'201612':'Z6',
'201701':'F7',
'201703':'H7',
'201705':'K7',
'201707':'N7'};

param_tag = "optionExpiration=";
nurl      = "http://198.56.183.230/getcme.php"
url       = "http://www.cmegroup.com/trading/agricultural/grain-and-oilseed/soybean_quotes_globex_options.html";
enc       = "UTF-8";
ul        = "optionQuotesUnderlierTable1_";
op        = "optionQuotesProductTable1_O";
quotecode = "component.futureContractQuoteCode = \"";


daily_file = sys.argv[1];
daily_fp   = open(daily_file, 'a');
csvfile    = csv.writer(daily_fp);

for mat in MaturityDates:
        ep = MaturityDates[mat];
        URL  = url + "?" + param_tag + ep;
        URL  = base64.b64encode(URL);
        req  = urllib2.Request(nurl+"?m="+URL);
        resp = urllib2.urlopen(req);
        respHtml = resp.read();
        soup = BeautifulSoup(respHtml, fromEncoding=enc);

        print respHtml;

        idx = respHtml.find(quotecode) + len(quotecode);
        code = respHtml[idx:idx+4];
        print mat,",",ep,",",code

        #MatDt,StrkPx,Type,Last,Change,Prior_Settle,High,Low,Volume,Hi_Low_Limit,Updated
        tmp = ul + code;
        last_id          = tmp + '_last';
        change_id        = tmp + '_change';
        prior_settle_id  = tmp + '_priorSettle';
        high_id          = tmp + '_high';
        low_id           = tmp + '_low';
        volume_id        = tmp + '_volume';
        hi_low_limit_id  = tmp + '_highLowLimits';
        update_time_id   = tmp + '_updated';

        ######################
        underlying_table = soup.find('table', {'id':'optionQuotesUnderlierTable1'});

        last         = str(underlying_table.find('td',{'id':last_id}).contents[0]);
        change       = str(underlying_table.find('td',{'id':change_id}).contents[0].contents[0]);
        prior_settle = str(underlying_table.find('td',{'id':prior_settle_id}).contents[0]);
        high         = str(underlying_table.find('td',{'id':high_id}).contents[0]);
        low          = str(underlying_table.find('td',{'id':low_id}).contents[0]);
        volume       = str(underlying_table.find('td',{'id':volume_id}).contents[0]);
        hi_low_limit = str(underlying_table.find('td',{'id':hi_low_limit_id}).contents[0]);
        update_time  = str(underlying_table.find('td',{'id':update_time_id}).contents[0].contents[0]);
        update_time  = update_time + str(underlying_table.find('td',{'id':update_time_id}).contents[0].contents[2]);

        last         = html_parser.unescape(last);
        change       = html_parser.unescape(change);
        prior_settle = html_parser.unescape(prior_settle);
        high         = html_parser.unescape(high);
        low          = html_parser.unescape(low);
        hi_low_limit = html_parser.unescape(hi_low_limit);

        csvfile.writerow([mat,'-','U',last,change,prior_settle,high,low,volume,hi_low_limit,update_time]);

        ######################
        option_table = soup.find('table',{'id':'optionQuotesProductTable1'});
        strikes_in_html = option_table.findAll('th',{'scope':'row'});

        idx = respHtml.find(op) + len(op);
        code = respHtml[idx:idx+4];
        print mat,",",ep,",",code

        strikes = [];
        for strike_price_in_html in strikes_in_html:
            tmp0 = str(strike_price_in_html.contents[0]);
            tmp1 = str(int(float(tmp0)))
            if len(tmp1) == 3:
                tmp1 = '0'+tmp1;
            strikes.append([tmp0, tmp1]);

        tp = 'C';
        for stkpx in strikes:
            tmp = op + code + '_' + tp + stkpx[1]; 
            print tmp;
            sys.stdout.flush();
            last_id          =  tmp + '_last';
            change_id        =  tmp + '_change'; 
            prior_settle_id  =  tmp + '_priorSettle';
            high_id          =  tmp + '_high';
            low_id           =  tmp + '_low';
            volume_id        =  tmp + '_volume';
            hi_low_limit_id  =  tmp + '_highLowLimits';
            update_time_id   =  tmp + '_updated';

            last             =  str(option_table.find('td',{'id':last_id}).contents[0]);
            change           =  str(option_table.find('td',{'id':change_id}).contents[0]);
            prior_settle     =  str(option_table.find('td',{'id':prior_settle_id}).contents[0]);
            high             =  str(option_table.find('td',{'id':high_id}).contents[0]);
            low              =  str(option_table.find('td',{'id':low_id}).contents[0]);
            volume           =  str(option_table.find('td',{'id':volume_id}).contents[0]);
            hi_low_limit     =  str(option_table.find('td',{'id':hi_low_limit_id}).contents[0]);
            update_time      =  str(option_table.find('td',{'id':update_time_id}).contents[0]);
            update_time      =  update_time + str(option_table.find('td',{'id':update_time_id}).contents[2]);

            last         = html_parser.unescape(last);
            change       = html_parser.unescape(change);
            prior_settle = html_parser.unescape(prior_settle);
            high         = html_parser.unescape(high);
            low          = html_parser.unescape(low);
            hi_low_limit = html_parser.unescape(hi_low_limit);

            csvfile.writerow([mat,stkpx[0],tp,last,change,prior_settle,high,low,volume,hi_low_limit,update_time]);


        tp = 'P';
        for stkpx in strikes:
            tmp = op + code + '_' + tp + stkpx[1]; 
            last_id          =  tmp + '_last';
            change_id        =  tmp + '_change'; 
            prior_settle_id  =  tmp + '_priorSettle';
            high_id          =  tmp + '_high';
            low_id           =  tmp + '_low';
            volume_id        =  tmp + '_volume';
            hi_low_limit_id  =  tmp + '_highLowLimits';
            update_time_id   =  tmp + '_updated';

            last             =  str(option_table.find('td',{'id':last_id}).contents[0]);
            change           =  str(option_table.find('td',{'id':change_id}).contents[0]);
            prior_settle     =  str(option_table.find('td',{'id':prior_settle_id}).contents[0]);
            high             =  str(option_table.find('td',{'id':high_id}).contents[0]);
            low              =  str(option_table.find('td',{'id':low_id}).contents[0]);
            volume           =  str(option_table.find('td',{'id':volume_id}).contents[0]);
            hi_low_limit     =  str(option_table.find('td',{'id':hi_low_limit_id}).contents[0]);
            update_time      =  str(option_table.find('td',{'id':update_time_id}).contents[0]);
            update_time      =  update_time + str(option_table.find('td',{'id':update_time_id}).contents[2]);

            last         = html_parser.unescape(last);
            change       = html_parser.unescape(change);
            prior_settle = html_parser.unescape(prior_settle);
            high         = html_parser.unescape(high);
            low          = html_parser.unescape(low);
            hi_low_limit = html_parser.unescape(hi_low_limit);

            csvfile.writerow([mat,stkpx[0],tp,last,change,prior_settle,high,low,volume,hi_low_limit,update_time]);

          

