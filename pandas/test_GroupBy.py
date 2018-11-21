import pandas as pd

def test1():
    df = pd.DataFrame({'a': [1, 2, 3, 4, 5, 6], 'b': [1, 1, 2, 3, 2, 8], 'c': [1, 4, 9, 16, 25, 36]})
    print df
    gb = df.groupby(['b'])
    print gb
    print "groupby size"
    # gb.size() return Series
    size = gb.size()
    print size

    gb.hist()

test1()
