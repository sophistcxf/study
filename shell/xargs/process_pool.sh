#########################################################################
# File Name: process_pool.sh
# Author: xiangfei.cui
# mail: xiangfei.cui@alibaba-inc.com
# Created Time: 二  3/13 17:53:17 2018
#########################################################################
#!/bin/bash

param=(`seq 100`)

for i in ${param[@]}
do
    sleep_time=$((RANDOM%6))
    cmd=${cmd}" processor.sh "${i}" "${sleep_time}
done

#echo "processor.sh 1 5" | xargs -n 3 sh 
PROC_CNT=4
echo ${cmd} | xargs -P ${PROC_CNT} -n 3 sh 
