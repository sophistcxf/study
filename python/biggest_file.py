#!/usr/bin/python

# coding: UTF-8

import os
import re
import sys

def biggest_file(path, pattern):
  '''
    get biggest file
  '''
  pattern = re.compile(pattern)
  max_filename = ""
  full_path = ""
  for parent, dirnames, filenames in os.walk(path):
    for filename in filenames:
      if not pattern.match(filename):
        continue
      if filename > max_filename:
        max_filename = filename
        full_path = os.path.join(parent, max_filename)
  return full_path

if __name__ == "__main__":
  if len(sys.argv) != 3:
    exit(1)
  print biggest_file(sys.argv[1], sys.argv[2])
  pass  
