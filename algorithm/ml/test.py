import pandas as pd

pd.set_option('display.max_columns', None)

columns = ['key', 'link', 'batch', 'state', 'features', 'weights']

df = pd.read_csv('test.txt', sep=',', names=columns)

d = {}
feat_cols = []

for f in df['features']:
    fds = f.strip().split(' ')
    for feat in fds:
        fds2 = feat.strip().split(':')
        idx = fds2[0]
        value = float(fds2[1])
        if idx not in d:
            d[idx] = []
            feat_cols.append(idx)
        d[idx].append(value)

d['label'] = list(df['state'].values)

df_feat = pd.DataFrame(d)

from compute_gain import compute_gain
from compute_gain import entropy
from compute_gain import gini_index
#compute_gain(df_feat, feat_cols, 'label', entropy)
compute_gain(df_feat, feat_cols, 'label', gini_index)
