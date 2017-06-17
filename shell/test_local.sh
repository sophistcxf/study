#########################################################################
# File Name: test_local.sh
# Author: cxxxxf
# mail: sophistcxf@gmail.com
# Created Time: Thu 11 Aug 2016 07:22:11 PM CST
#########################################################################
#!/bin/bash

# For  each  argument,  a  local  variable  named  name  is created, and assigned value

function f()
{
  local a="a"
  echo $a
}

function f2()
{
  b="b"
  echo $b
}

f
echo $a

f2
echo $b
