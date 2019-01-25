#! -*- coding=utf-8 -*-
import collections as clt


def test_Counter():
    a = 'hello world, i\'m cxxxxf. nice to meet you'
    c = clt.Counter(a)
    help(c)

    # 遍历，多种遍历方式
    for k in c:
        print k, c[k]

    for k,v in c.items():
        print k,v

    # 最常出现的 3 个
    print c.most_common(3)

    print c.keys()
    print c.values()

    print c.viewkeys()
    print c.viewvalues()

if __name__ == "__main__":
    test_Counter()
