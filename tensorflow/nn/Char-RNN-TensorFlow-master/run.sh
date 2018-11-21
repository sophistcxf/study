#########################################################################
# File Name: run.sh
# Author: xiangfei.cui
# mail: xiangfei.cui@alibaba-inc.com
# Created Time: 三  9/ 5 22:12:38 2018
#########################################################################
#!/bin/bash

train_chinese_poetry()
{
python train.py \
  --use_embedding \
  --input_file data/poetry.txt \
  --name poetry \
  --learning_rate 0.005 \
  --num_steps 26 \
  --num_seqs 32 \
  --max_steps 10000
}

sample_chinese_poetry()
{
python sample.py \
  --use_embedding \
  --converter_path model/poetry/converter.pkl \
  --checkpoint_path model/poetry/ \
  --max_length 300
}

sample_chinese_poetry
