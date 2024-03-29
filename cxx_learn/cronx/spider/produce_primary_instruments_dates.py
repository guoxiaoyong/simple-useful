#!/usr/bin/python
# -*- coding: utf-8 -*-
import os;
import sys;
import datetime;
import string;
import shutil;

EPSILON = 1e-10;
Product_volume_dict = {};
START_TIME='09:20:00';
END_TIME='14:55:00';

if len(sys.argv) != 5:
    print 'Usage: ./cmd src_md_dir dest_dir from_date(YYYYMMHH) to_date(YYYYMMHH)';
    quit();

src_md_path = sys.argv[1];
dest_dir = sys.argv[2];
from_date_dir = sys.argv[3];
to_date_dir = sys.argv[4];
date_dir = from_date_dir;
while date_dir <= to_date_dir:
    if not os.path.exists(src_md_path+'/'+date_dir):
        date_dir = (datetime.datetime.strptime(date_dir,'%Y%m%d')+datetime.timedelta(1)).strftime('%Y%m%d');
        continue;
    Product_volume_dict = {};
    dest_daily_dir = dest_dir+'/'+date_dir;
    if not os.path.exists(dest_daily_dir):
        os.mkdir(dest_daily_dir);
    daily_md_path = src_md_path+'/'+date_dir;
    daily_md_list = os.listdir(daily_md_path);
    for md_file_zip in daily_md_list:
        if md_file_zip.find('_') >=0 or md_file_zip.find('.gz') == -1:
            continue;
        md_file_zip_path = daily_md_path+'/'+md_file_zip;
        dest_daily_md_path = dest_daily_dir+'/'+md_file_zip;
        shutil.copyfile(md_file_zip_path, dest_daily_md_path);
        os.system('gzip -d '+dest_daily_md_path);
        dest_daily_md_path = dest_daily_md_path[0:len(dest_daily_md_path)-3];
        digit_index=0;
        for c in md_file_zip:
            if c>='0' and c<='9':
                break;
            digit_index = digit_index+1;
        if digit_index == len(md_file_zip):
            os.remove(dest_daily_md_path);
            continue;
        product_name = md_file_zip[0:digit_index];
        #print product_name;
        #continue;
        dest_fp = open(dest_daily_md_path, 'r');
        md_lines = dest_fp.readlines();
        dest_fp.close();
        #print dest_daily_md_path,':',len(md_lines);
        if len(md_lines) == 0:
            os.remove(dest_daily_md_path);
            continue;
        begin_line = md_lines[0];
        begin_line_list = begin_line.split(',');
        close_line = md_lines[-1];
        close_line_list = close_line.split(',');
        if begin_line_list[1]>START_TIME or close_line_list[1]<END_TIME:
            os.remove(dest_daily_md_path);
            continue;
        close_volume = string.atof(close_line_list[3]);
        last_volume = Product_volume_dict.get(product_name);
        if last_volume == None or close_volume > last_volume:
            Product_volume_dict[product_name] = close_volume;
            os.rename(dest_daily_md_path,dest_daily_dir+'/'+product_name+'.csv');
        else:
            os.remove(dest_daily_md_path);
    print 'Finish processing',date_dir;
    date_dir = (datetime.datetime.strptime(date_dir,'%Y%m%d')+datetime.timedelta(1)).strftime('%Y%m%d');
