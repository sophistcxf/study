Multiclass Classification Example
=================================

Here is an example for LightGBM to run multiclass classification task.

***You should copy executable file to this folder first.***

Training
--------

Run the following command in this folder:

```
"./lightgbm" config=train.conf
```

Prediction
----------

You should finish training first.

Run the following command in this folder:

```
"./lightgbm" config=predict.conf
```

文件列表
---------
训练配置 train.conf
预测配置 predict.conf
训练数据 multiclass.train
预测数据 multiclass.test

生成的模型 LightGBM_model.txt
生成的预测结果 LightGBM_predict_result.txt
