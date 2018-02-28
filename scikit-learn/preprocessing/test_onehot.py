from sklearn import preprocessing

enc = preprocessing.OneHotEncoder()
enc.fit([[1], [2], [3], [4], [1], [5]])
print enc.transform([[3], [4]]).toarray()
