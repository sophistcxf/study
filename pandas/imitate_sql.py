#! -*- coding=utf-8 -*-

import pandas as pd
import numpy as np

def test_sql():
    url = 'https://raw.github.com/pandas-dev/pandas/master/pandas/tests/data/tips.csv'
    tips = pd.read_csv(url)

    # select total_bill, tip, smoker from tips limit 5
    print tips[['total_bill', 'tip', 'smoker']].head(5)

    df1 = pd.DataFrame({'key': ['A', 'B', 'C', 'D'], 'key2':['A', 'B', 'C', 'D'], 'value': np.random.randn(4)})
    df2 = pd.DataFrame({'key': ['B', 'D', 'D', 'E'], 'key2':['B', 'D', 'D', 'E'], 'value': np.random.randn(4)})
    print df1
    print df2

    # select * from df1 join df2 on df1.key=df2.key and df1.key2=df2.key2
    print df1.merge(df2, on=['key', 'key2'])

    # select * from df1 left join df2 on df1.key=df2.key and df1.key2=df2.key2
    print df1.merge(df2, on=['key', 'key2'], how='left')

    # select * from df1 right join df2 on df1.key=df2.key and df1.key2=df2.key2
    print df1.merge(df2, on=['key', 'key2'], how='right')

    # select * from df1 outer join df2 on df1.key=df2.key and df1.key2=df2.key2
    print df1.merge(df2, on=['key', 'key2'], how='outer')

    # 参数 on 要求 df1 和 df2 都有此字段，如果想用不同的字段，可用 left_on 和 right_on
    print df1.merge(df2, left_on=['key'], right_on=['key2'], how='inner')

if __name__ == "__main__":
    test_sql()
