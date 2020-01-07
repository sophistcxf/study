#coding=utf-8

#通过minidom解析xml文件
import xml.dom.minidom as xmldom
import os
''' 
XML文件读取 
<?xml version="1.0" encoding="utf-8"?>
<catalog>
    <maxid>4</maxid>
    <login username="pytest" passwd='123456'>dasdas
        <caption>Python</caption>
        <item id="4">
            <caption>测试</caption>
        </item>
    </login>
    <item id="2">
        <caption>Zope</caption>
    </item>
</catalog>

'''

xmlfilepath = os.path.abspath("sample.xml")
print ("xml文件路径：", xmlfilepath)

# 得到文档对象
domobj = xmldom.parse(xmlfilepath)
print("xmldom.parse:", type(domobj))
# 得到元素对象
elementobj = domobj.documentElement
print ("domobj.documentElement:", type(elementobj))

#获得子标签
subElementObj = elementobj.getElementsByTagName("route")
print ("getElementsByTagName:", type(subElementObj))

print elementobj.getAttribute('NaviID')

subElementObj = elementobj.getElementsByTagName('startpoint')
print subElementObj[0].getElementsByTagName('x')[0].firstChild.data
print subElementObj[0].getElementsByTagName('y')[0].firstChild.data

subElementObj = elementobj.getElementsByTagName('endpoint')
print subElementObj[0].getElementsByTagName('x')[0].firstChild.data
print subElementObj[0].getElementsByTagName('y')[0].firstChild.data
