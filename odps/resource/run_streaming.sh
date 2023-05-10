#########################################################################
# File Name: run_streaming.sh
# Author: xiangfei.cui
# mail: xiangfei.cui@alibaba-inc.com
# Created Time: Sun 19 May 2019 10:18:52 PM CST
#########################################################################
#!/bin/bash



set -x
set -u
set -e

odpscmd -e 'jar
 com.aliyun.odps.mapred.bridge.streaming.StreamJob
 -partitioner com.aliyun.odps.mapred.lib.KeyFieldBasedPartitioner
 -jobconf odps.stage.mapper.split.size=64
 -jobconf stream.num.map.output.key.fields=5
 -jobconf odps.stage.reducer.num=4713
 -jobconf num.key.fields.for.partition=3
 -mapper "sh mapper.sh"
 -reducer "sh reducer.sh"
'
