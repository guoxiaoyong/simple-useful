#!/usr/bin/python
# -*- coding: utf-8 -*-
import os;
import sys;
import datetime;
import string;
import shutil;

EPSILON = 1e-10;
Product_volume_dict = {};
START_TIME='20:55:00';
END_TIME='02:45:00';

if len(sys.argv) != 3:
    print 'Usage: ./cmd src_md_dir dest_dir';
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
    Product_volume_dict = {};
    dest_night_dir = dest_dir+'/'+date_dir;
    if not os.path.exists(dest_night_dir):
        os.mkdir(dest_night_dir);
    night_md_path = src_md_path+'/'+date_dir;
    night_md_list = os.listdir(night_md_path);
    for night_md_file in night_md_list:
        if night_md_file.find('_n.csv') == -1:
            continue;
        night_md_file_path = night_md_path+'/'+night_md_file;
        dest_night_md_path = dest_night_dir+'/'+night_md_file;
        shutil.copyfile(md_file_zip_path, dest_night_md_path);
        os.system('gzip -d '+dest_night_md_path);
        dest_night_md_path = dest_night_md_path[0:len(dest_night_md_path)-3];
        digit_index=0;
        for c in md_file_zip:
            if c>='0' and c<='9':
                break;
            digit_index = digit_index+1;
        if digit_index == len(md_file_zip):
            os.remove(dest_night_md_path);
            continue;
        product_name = md_file_zip[0:digit_index];
        #print product_name;
        #continue;
        dest_fp = open(dest_night_md_path, 'r');
        md_lines = dest_fp.readlines();
        dest_fp.close();
        #print dest_night_md_path,':',len(md_lines);
        if len(md_lines) == 0:
            os.remove(dest_night_md_path);
            continue;
        begin_line = md_lines[0];
        begin_line_list = begin_line.split(',');
        close_line = md_lines[-1];
        close_line_list = close_line.split(',');
        if begin_line_list[1]>START_TIME or close_line_list[1]<END_TIME:
            os.remove(dest_night_md_path);
            continue;
        close_volume = string.atof(close_line_list[3]);
        volumes_list = Product_volume_dict.get(product_name);
        if volumes_list == None:
            volumes_list = [];
            volumes_list.append(close_volume);
            Product_volume_dict[product_name] = volumes_list;
            os.rename(dest_night_md_path,dest_night_dir+'/'+product_name+'.csv');
        elif len(volumes_list)==1:
            if close_volume>volumes_list[0]:
                volumes_list.append(volumes_list[0]);
                volumes_list[0] = close_volume;
                os.rename(dest_night_dir+'/'+product_name+'.csv',dest_night_dir+'/'+product_name+'2.csv');
                os.rename(dest_night_md_path,dest_night_dir+'/'+product_name+'.csv');
            else:
                volumes_list.append(close_volume);
                os.rename(dest_night_md_path,dest_night_dir+'/'+product_name+'2.csv');
        else:
            if close_volume>volumes_list[0]:
                volumes_list[1] = volumes_list[0];
                volumes_list[0] = close_volume;
                os.rename(dest_night_dir+'/'+product_name+'.csv',dest_night_dir+'/'+product_name+'2.csv');
                os.rename(dest_night_md_path,dest_night_dir+'/'+product_name+'.csv');
            elif close_volume>volumes_list[1]:
                volumes_list[1] = close_volume;
                os.rename(dest_night_md_path,dest_night_dir+'/'+product_name+'2.csv');
            else:
                os.remove(dest_night_md_path);
    print 'Finish processing',date_dir;
    date_dir = (datetime.datetime.strptime(date_dir,'%Y%m%d')+datetime.timedelta(1)).strftime('%Y%m%d');
