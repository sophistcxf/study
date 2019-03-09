#! -*- coding=utf-8 -*-
def test_view():
    '''
    如果使用 viewkeys 和 viewvalues 获得 keys 和 values，当删除掉 dict 中的元素时，相应的
    view_keys 和 view_values 中的元素也会被删掉
    所 https://stackoverflow.com/questions/8957750/what-are-dictionary-view-objects 说，python2 和 python3 已经不同了
    '''
    dishes = {'eggs': 2, 'sausage': 1, 'bacon': 1, 'spam': 500}
    keys = dishes.keys()
    values = dishes.values()
    print keys
    print values
    del dishes['eggs']
    print keys
    print values

    dishes = {'eggs': 2, 'sausage': 1, 'bacon': 1, 'spam': 500}
    view_keys = dishes.viewkeys()
    view_values = dishes.viewvalues()
    print view_keys
    print view_values
    del dishes['eggs']
    print view_keys
    print view_values


def test_asterisk():
    def func(a, b, c):
        print a,b,c

    # 说明这样调用参数是有对应的，不需要传参时按照函数声明时的顺序
    # 输出 1 2 3
    d = {'a':1, 'b':2, 'c':3}
    func(**d)

    # 输出 1 3 2
    d = {'a':1, 'c':2, 'b':3}
    func(**d)


    # 由于 func 参数中没有 e，因而会报错
    d = {'a':1, 'c':2, 'e':3}
    func(**d)

if __name__ == "__main__":
    #test_view()
    test_asterisk()
