#########################################################################
# File Name: test_array.sh
# Author: cxxxxf
# mail: sophistcxf@gmail.com
# Created Time: Thu 28 Jul 2016 02:44:59 PM CST
#########################################################################
#!/bin/bash

# convert string to array
str="1 2 3 4 5"
arr=($str)
echo ${#arr[@]}

# iterate
echo iterate
arr=(1 2 3 4 5)
for i in ${arr[@]}
do
  echo $i
done

for ((i = 0; i < ${#arr[@]}; i++))
do
  echo ${arr[$i]}
done

# delete first 2
arr=(1 2 3 4 5)
echo ${arr[@]:2}

# print 
echo ${arr[@]}

# length
echo ${#arr[@]}

# modify
arr=(1 2 3 4 5)
arr[3]=10
echo ${arr[@]}

arr=(abcthredcc threed threabc)
# delete shortest matching from left
echo ${arr[@]#t*e}
# delete longest matching from left
echo ${arr[@]##t*e}
