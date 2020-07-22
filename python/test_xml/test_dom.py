#coding=utf-8

#通过minidom解析xml文件
import xml.dom.minidom as xmldom
import os

def test1():
    xmlfilepath = os.path.abspath("test.xml")
    print ("xml文件路径：", xmlfilepath)

    # 得到文档对象
    domobj = xmldom.parse(xmlfilepath)
    print("xmldom.parse:", type(domobj))
    # 得到元素对象
    elementobj = domobj.documentElement
    print ("domobj.documentElement:", type(elementobj))

    #获得子标签
    subElementObj = elementobj.getElementsByTagName("login")
    print ("getElementsByTagName:", type(subElementObj))

    print (len(subElementObj))
    # 获得标签属性值
    print (subElementObj[0].getAttribute("username"))
    print (subElementObj[0].getAttribute("passwd"))

    #区分相同标签名的标签
    subElementObj1 = elementobj.getElementsByTagName("caption")
    for i in range(len(subElementObj1)):
        print ("subElementObj1[i]:", type(subElementObj1[i]))
        print (subElementObj1[i].firstChild.data)  #显示标签对之间的数据

def test2():
    '''
    解析文本的 xml 内容
    '''
    con = open('test.xml', 'r').read()

    print 'xml is'
    print con

    '''
    domobj 是 Document 对象
    '''
    domobj = xmldom.parseString(con)
    print("xmldom.parse:", type(domobj))

    '''
    elementobj 是 Element 对象
    '''
    elementobj = domobj.documentElement
    print ("domobj.documentElement:", type(elementobj))

    '''
    subElementObj 是一个 xml.dom.minicompat.NodeList
    '''
    subElementObj = elementobj.getElementsByTagName("login")
    print ("getElementsByTagName:", type(subElementObj))

    print (len(subElementObj))
    # 获得标签属性值
    print (subElementObj[0].getAttribute("username"))
    print (subElementObj[0].getAttribute("passwd"))

    '''
    caption 这个标签在多级下都有，会把这个标签依次打出来
    '''
    print 'caption 这个标签在多级下都有'
    subElementObj1 = elementobj.getElementsByTagName("caption")
    for i in range(len(subElementObj1)):
        print ("subElementObj1[i]:", type(subElementObj1[i]))
        print (subElementObj1[i].firstChild.data)  #显示标签对之间的数据

    '''
    只想拿到 login 下的第一级 caption
    '''
    subElementObj = elementobj.getElementsByTagName("login")
    login_caption = subElementObj[0].getElementsByTagName('caption')
    help(login_caption[0])

def insight_dom():
    '''
    探究 dom 内部
    '''

test2()
