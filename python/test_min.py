d = {'a':1, 'b':2, 'c':3, 'd':-1, 'e':2, 'f':100, 'g':-20}

l = [-1, -2, 1, 2, 3]

print min(d)
print min(l)
print min(d, l)

'''
key 是指定排序函数，min 内部会调用 key=func，用得到的结果进行排序，大概是
min_value, min_item
for i in arg:
    v = func(i)
    if v < min_item:
        min_value = v
        min_item = i
return min_item
'''
print min(d, key=d.get)
