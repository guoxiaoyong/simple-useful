import sys

if sys.platform == 'darwin':
  src='/Volumes/TOSHIBA EXT/体育项目管理'
  dst='/Users/xguo/xiaoying/体育项目管理'
else:
  src='/run/media/guo/TOSHIBA EXT'
  dst='/run/media/guo/TOSHIBA EXT2/xiaoying'

with open(sys.argv[1], encoding='utf8') as infile:
  filenames = infile.readlines()
  filenames = [fn.strip() for fn in filenames]

cmd = 'cp -r "%s/%s" "%s/"'
for fn in filenames:
  print(cmd % (src, fn, dst))
