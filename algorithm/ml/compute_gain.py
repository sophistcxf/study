# coding=utf-8

'''
计算各个特征的信息增益
'''

import pandas as pd
from scipy import stats
import sys

def entropy(se):
    l = [i * 1. / se.shape[0] for i in se.value_counts()]
    return stats.entropy(l)

def gini_index(se):
    l = [i * 1. / se.shape[0] for i in se.value_counts()]
    return 1 - sum([pow(i,2) for i in l])

def compute_feat_gain(df, feat_col, label_col, impurity_func):
    feat_df = df[[feat_col, label_col]]
    if max(df[feat_col].values) > 1000:
        feat_se = list(pd.Series(df[feat_col].values).apply(lambda x:int(x/10)*10).sort_values().unique())
    elif max(df[feat_col].values) > 100:
        feat_se = list(pd.Series(df[feat_col].values).apply(lambda x:int(x)).sort_values().unique())
    else:
        feat_se = list(pd.Series(df[feat_col].values).apply(lambda x:round(x,1)).sort_values().unique())
    seg_points = [ (feat_se[i] + feat_se[i+1]) / 2 for i in range(len(feat_se)-1)]
    h_d = impurity_func(pd.Series(df[label_col].values))
    max_gain = 0
    best_seg_pnt = None
    for pnt in seg_points:
        df_feat_low = feat_df[feat_df[feat_col]<=pnt] 
        r1 = df_feat_low.shape[0] * 1. / feat_df.shape[0]
        h_d1 = impurity_func(pd.Series(df_feat_low[label_col].values))
        df_feat_high = feat_df[feat_df[feat_col]>pnt]
        r2 = df_feat_high.shape[0] * 1. / feat_df.shape[0]
        h_d2 = impurity_func(pd.Series(df_feat_high[label_col].values))
        gain = h_d - (r1 * h_d1 + r2 * h_d2)
        if gain > max_gain:
            max_gain = gain
            best_seg_pnt = pnt
    return (max_gain, best_seg_pnt)

def compute_gain(df, feature_cols, label_col, impurity_func):
    '''
    计算各个特征的信息增益
    '''
    columns=['feat_name', 'gain', 'seg_pnt']
    df_rlt = pd.DataFrame(columns=['feat_name', 'gain', 'seg_pnt'])
    for feat_col in feature_cols:
        print >> sys.stderr, "beg compute %s..." % feat_col
        (gain, seg_pnt) = compute_feat_gain(df, feat_col, label_col, impurity_func)
        df_rlt = df_rlt.append(pd.DataFrame({'feat_name':[feat_col], 'gain':[gain], 'seg_pnt':[seg_pnt]}), ignore_index=True)
        print >> sys.stderr, "end compute %s..." % feat_col
    print df_rlt.sort_values(by=['gain'], ascending=False)

def test_iris():
    from sklearn.datasets import load_iris
    iris = load_iris();
    print iris.data.shape
    print iris.target.shape
    print iris.feature_names
    print iris.target_names
    df = pd.DataFrame(iris.data, columns=iris.feature_names)
    df['label'] = iris.target
    compute_gain(df, iris.feature_names, 'label', gini_index)
    #compute_gain(df, iris.feature_names, 'label', entropy)

if __name__ == "__main__":
    test_iris()

