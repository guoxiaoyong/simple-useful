#!/usr/bin/python
# -*- coding: utf-8 -*-
import urllib2;
import sys;
import datetime;
import time;
import threading;
import os;
from BeautifulSoup import BeautifulSoup;

LatestPriceIndex =    3;
VolumeIndex =         8;
TurnoverIndex =       9;
AskPrice1Index =      21;
AskVolume1Index =     20;
AskPrice2Index =      23;
AskVolume2Index =     22;
AskPrice3Index =      25;
AskVolume3Index =     24;
AskPrice4Index =      27;
AskVolume4Index =     26;
AskPrice5Index =      29;
AskVolume5Index =     28;
BidPrice1Index =      11;
BidVolume1Index =     10;
BidPrice2Index =      13;
BidVolume2Index =     12;
BidPrice3Index =      15;
BidVolume3Index =     14;
BidPrice4Index =      17;
BidVolume4Index =     16;
BidPrice5Index =      19;
BidVolume5Index =     18;
DayIndex       =      30;
TimeStampIndex =      31;

Title_str = "TimeStamp,LatestPrice,\
Volume(share),Turnover(CNY),AskPrice1,AskVolume1,\
AskPrice2,AskVolume2,AskPrice3,AskVolume3,\
AskPrice4,AskVolume4,AskPrice5,AskVolume5,\
BidPrice1,BidVolume1,BidPrice2,BidVolume2,\
BidPrice3,BidVolume3,BidPrice4,BidVolume4,\
BidPrice5,BidVolume5\n";

Title_str_csi300 = "TimeStamp,LatestPrice,\
Volume(100share),Turnover(CNY),AskPrice1,AskVolume1,\
AskPrice2,AskVolume2,AskPrice3,AskVolume3,\
AskPrice4,AskVolume4,AskPrice5,AskVolume5,\
BidPrice1,BidVolume1,BidPrice2,BidVolume2,\
BidPrice3,BidVolume3,BidPrice4,BidVolume4,\
BidPrice5,BidVolume5\n";
CSI_Name = "沪深300指数_sh000300";

HtmlEncoding = "GB2312";

Morning_ended = 1;
Afternoon_ended = 2;
DuringTrading = 3;
ReadyRun = 4;

class SecurityMdGrabber:

    def __init__(self, str_name, str_url):
        global md_dir;
        global Title_str;
        global Title_str_csi300;
        global start_time0;

        file_path = today_md_dir + '/' + str_name + '.csv';
        if os.path.exists(file_path):
            self.__md_fp = open(file_path, 'a');
        else:
            self.__md_fp = open(file_path, 'w');
            if str_name == CSI_Name:
                self.__md_fp.write(Title_str_csi300);
            else:
                self.__md_fp.write(Title_str);
            self.__md_fp.flush();
        self.__str_url = str_url;
        self.__req = urllib2.Request(str_url); 
        self.__timestamp = start_time0 - datetime.timedelta(seconds=1);

    def __del__(self):
        self.__md_fp.close();

    def flush_md(self):
        self.__md_fp.flush();

    def reset(self):
        self.__status = ReadyRun;

    def get_md(self):
        global HtmlEncoding;
        global Morning_ended;
        global Afternoon_ended;
        global str_today;
        global start_time0;
        global end_time0;
        global start_time1;
        global end_time1;
        
        """
        if self.__status == Morning_ended or self.__status == Afternoon_ended:
            return self.__status;
        """
        try:
            print datetime.datetime.now().isoformat(),":before urlopen:",self.__str_url;
            resp = urllib2.urlopen(self.__req,timeout=5);
            print datetime.datetime.now().isoformat(),":after urlopen:",self.__str_url;
        except:
            print datetime.datetime.now().isoformat(),": While access url",self.__str_url,"caught an exception";
            return DuringTrading;
        respHtml=resp.read();
        soup = BeautifulSoup(respHtml, fromEncoding=HtmlEncoding);
        try:
            md_list = soup.contents[0].split(',');
            time_stamp = datetime.datetime.strptime(md_list[DayIndex] + ' '  + md_list[TimeStampIndex], "%Y-%m-%d %H:%M:%S");
        except:
            print datetime.datetime.now().isoformat(),": While access url",self.__str_url,": split error: md_list = ",md_list,"and soup = ",soup.contents[0];
            return DuringTrading;
        print datetime.datetime.now().isoformat(),": md_grabber:",self.__str_url,": got tick with timestamp: ", time_stamp.isoformat();
        if time_stamp <= end_time0:
            if time_stamp > self.__timestamp:
                tick_str = md_list[TimeStampIndex] + ',' + md_list[LatestPriceIndex]\
                                                 + ',' + md_list[VolumeIndex] + ',' + md_list[TurnoverIndex]\
                                                 + ',' + md_list[AskPrice1Index] + ',' + md_list[AskVolume1Index]\
                                                 + ',' + md_list[AskPrice2Index] + ',' + md_list[AskVolume2Index]\
                                                 + ',' + md_list[AskPrice3Index] + ',' + md_list[AskVolume3Index]\
                                                 + ',' + md_list[AskPrice4Index] + ',' + md_list[AskVolume4Index]\
                                                 + ',' + md_list[AskPrice5Index] + ',' + md_list[AskVolume5Index]\
                                                 + ',' + md_list[BidPrice1Index] + ',' + md_list[BidVolume1Index]\
                                                 + ',' + md_list[BidPrice2Index] + ',' + md_list[BidVolume2Index]\
                                                 + ',' + md_list[BidPrice3Index] + ',' + md_list[BidVolume3Index]\
                                                 + ',' + md_list[BidPrice4Index] + ',' + md_list[BidVolume4Index]\
                                                 + ',' + md_list[BidPrice5Index] + ',' + md_list[BidVolume5Index]\
                                                 + '\n';
                self.__md_fp.write(tick_str);
                self.__md_fp.flush();
                self.__timestamp = time_stamp;
            if time_stamp == end_time0:
                self.__status = Morning_ended;
                return Morning_ended;
            else:
                return DuringTrading;

        elif time_stamp < start_time1:
            self.__status = Morning_ended;
            return Morning_ended;

        elif time_stamp <= end_time1:
            if time_stamp > self.__timestamp:
                tick_str = md_list[TimeStampIndex] + ',' + md_list[LatestPriceIndex]\
                                                 + ',' + md_list[VolumeIndex] + ',' + md_list[TurnoverIndex]\
                                                 + ',' + md_list[AskPrice1Index] + ',' + md_list[AskVolume1Index]\
                                                 + ',' + md_list[AskPrice2Index] + ',' + md_list[AskVolume2Index]\
                                                 + ',' + md_list[AskPrice3Index] + ',' + md_list[AskVolume3Index]\
                                                 + ',' + md_list[AskPrice4Index] + ',' + md_list[AskVolume4Index]\
                                                 + ',' + md_list[AskPrice5Index] + ',' + md_list[AskVolume5Index]\
                                                 + ',' + md_list[BidPrice1Index] + ',' + md_list[BidVolume1Index]\
                                                 + ',' + md_list[BidPrice2Index] + ',' + md_list[BidVolume2Index]\
                                                 + ',' + md_list[BidPrice3Index] + ',' + md_list[BidVolume3Index]\
                                                 + ',' + md_list[BidPrice4Index] + ',' + md_list[BidVolume4Index]\
                                                 + ',' + md_list[BidPrice5Index] + ',' + md_list[BidVolume5Index]\
                                                 + '\n'; 
                self.__md_fp.write(tick_str);
                self.__md_fp.flush();
                self.__timestamp = time_stamp;
            
            if time_stamp == end_time1:
                    self.__status = Afternoon_ended;
                    return Afternoon_ended;
            else:
                return DuringTrading;
        
        else:
            self.__status = Afternoon_ended;
            return Afternoon_ended;

class MdThread(threading.Thread):
    def __init__(self, grabber_list):
        threading.Thread.__init__(self);
        self.__grabber_list = grabber_list;
        self.__list_len = len(grabber_list);
    
    def run(self):
        for i in range(0, self.__list_len):
            self.__grabber_list[i].reset();
        while True:
            ended_number = 0;
            for i in range(0, self.__list_len):
                ret = self.__grabber_list[i].get_md();
                if ret != DuringTrading:
                    ended_number = ended_number + 1;
            if ended_number == self.__list_len:
                if ret == Morning_ended:
                    for i in range(0, self.__list_len):
                        self.__grabber_list[i].flush_md();
                    current_time = datetime.datetime.now();
                    if current_time < start_time1:
                        sleep_interval = (start_time1 - current_time).seconds;
                        print self.getName(), 'begin to sleep with interval =',sleep_interval,\
                        "(",start_time1.isoformat(),'-',current_time.isoformat(),")";
                        time.sleep(sleep_interval);
                        print self.getName(), 'wakeup at',datetime.datetime.now();
                    for i in range(0, self.__list_len):
                        self.__grabber_list[i].reset();
                else:
                    break;
            time.sleep(1);
 
print datetime.datetime.now().isoformat(),": spider_realtime begin to run"; 
str_start_time0 = "09:30:00";
str_end_time0 = "11:31:00";
str_start_time1 = "13:00:00";
str_end_time1 = "15:01:00";

today = datetime.date.today();
str_today = today.strftime('%Y%m%d');
start_time0 = datetime.datetime.strptime(str_today + ' ' + str_start_time0, "%Y%m%d %H:%M:%S");
end_time0 = datetime.datetime.strptime(str_today + ' ' + str_end_time0, "%Y%m%d %H:%M:%S");
start_time1 = datetime.datetime.strptime(str_today + ' ' + str_start_time1, "%Y%m%d %H:%M:%S");
end_time1 = datetime.datetime.strptime(str_today + ' ' + str_end_time1, "%Y%m%d %H:%M:%S");

config_file = sys.argv[1];
md_dir = sys.argv[2];
if not os.path.exists(md_dir):
    os.mkdir(md_dir);

today_md_dir = md_dir + '/' + str_today;
if not os.path.exists(today_md_dir):
    os.mkdir(today_md_dir);

config_fp = open(config_file, 'r');

thread_list = [];
grabber_list = [];
line_numbers = 0;
config_line = config_fp.readline();
while len(config_line) > 0:    
    line_list = config_line.split(',');
    str_name = line_list[0];
    str_url = line_list[1].strip();
    grabber_list.append(SecurityMdGrabber(str_name, str_url));
    line_numbers = line_numbers+1;
    if line_numbers == 20:
        thread_list.append(MdThread(grabber_list));
        line_numbers = 0;
        grabber_list = [];
    config_line = config_fp.readline();
if line_numbers > 0:
    thread_list.append(MdThread(grabber_list));
    grabber_list = [];
config_fp.close();

for i in range(0, len(thread_list)):
    thread_list[i].start();
for i in range(0, len(thread_list)):
    thread_list[i].join();
    print "thread -",i+1,"ended";
