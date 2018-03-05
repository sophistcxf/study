from sklearn import preprocessing

enc = preprocessing.OneHotEncoder()
enc.fit([[1,4], [2,2], [3,10], [4,6], [1,7], [5,3]])
#print enc.transform([[3,6], [4,7]]).toarray()

enc = preprocessing.OneHotEncoder([6, 11])
enc.fit([[1,4], [2,2], [3,10], [4,6], [1,7], [5,3]])
print enc.transform([[3, 2],[4, 4]])
