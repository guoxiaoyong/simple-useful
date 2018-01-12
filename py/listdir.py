import os
import stat

def listall(pathname):
  files_in_curr_dir = [
      os.path.join(pathname, file_or_dir) for
      file_or_dir in os.listdir(pathname)]
  files = []
  for filename in files_in_curr_dir:
    if os.path.isfile(filename):
      files.append(filename)
    else os.path.isdir(filename):
      files.extend(listall(filename))
  return files
