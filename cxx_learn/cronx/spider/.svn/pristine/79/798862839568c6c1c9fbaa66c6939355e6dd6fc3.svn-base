#!/usr/bin/python
# -*- coding: utf-8 -*-
import os;
import sys;
import datetime;
import string;
import shutil;

START_TIME='09:40:00';
END_TIME='14:55:00';

if len(sys.argv) != 3:
    print 'Usage: ./cmd src_md_dir dest_dir ';
    quit();

src_md_path = sys.argv[1];
dest_dir = sys.argv[2];
from_date_dir = datetime.date.today().strftime('%Y%m%d');
to_date_dir = from_date_dir;
date_dir = from_date_dir;
while date_dir <= to_date_dir:
    if not os.path.exists(src_md_path+'/'+date_dir):
        date_dir = (datetime.datetime.strptime(date_dir,'%Y%m%d')+datetime.timedelta(1)).strftime('%Y%m%d');
        continue;
    dest_daily_dir = dest_dir+'/'+date_dir;
    if not os.path.exists(dest_daily_dir):
        os.mkdir(dest_daily_dir);
    daily_md_path = src_md_path+'/'+date_dir;
    daily_md_list = os.listdir(daily_md_path);
    sh000300_file = "";
    for daily_md_file in daily_md_list:
        if daily_md_file.find('sh000300')>=0:
            sh000300_file = daily_md_file;
            break;
    if len(sh000300_file) == 0:
        date_dir = (datetime.datetime.strptime(date_dir,'%Y%m%d')+datetime.timedelta(1)).strftime('%Y%m%d');
        continue;
    src_file_path = daily_md_path+'/'+daily_md_file;
    src_fp = open(src_file_path, 'r');
    md_lines = src_fp.readlines();
    if len(md_lines) <=1:
        date_dir = (datetime.datetime.strptime(date_dir,'%Y%m%d')+datetime.timedelta(1)).strftime('%Y%m%d');
        src_fp.close();
        continue;
    begin_line = md_lines[1];
    close_line = md_lines[-1];
    begin_line_list = begin_line.split(',');
    close_line_list = close_line.split(',');
    if begin_line_list[0]>START_TIME or close_line_list[0]<END_TIME:
        date_dir = (datetime.datetime.strptime(date_dir,'%Y%m%d')+datetime.timedelta(1)).strftime('%Y%m%d');
        src_fp.close();
        continue;
    dest_file_path = dest_daily_dir+'/'+'sh000300.csv';
    dest_fp = open(dest_file_path, 'w');
    for i in range(1,len(md_lines)):
        md_line_list = md_lines[i].split(',');
        write_line = 'sh000300,'+md_line_list[0]+','+md_line_list[1]+'\n';
        dest_fp.write(write_line);
    src_fp.close();
    dest_fp.close();
    print 'Finish processing',date_dir;
    date_dir = (datetime.datetime.strptime(date_dir,'%Y%m%d')+datetime.timedelta(1)).strftime('%Y%m%d');
