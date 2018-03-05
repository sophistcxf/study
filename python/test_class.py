# -*- coding:utf-8 -*-
# file_name: python_class_inheritance_example.py
class Animal(object):
    def __init__(self, name):
        self.name = name
 
    def saySomething(self):
        print("I am " + self.name)
 
class Dog(Animal):
    def __init__(self, name):
        super(Dog, self).__init__(name)
 
    def saySomething(self):
        print ("I am "+ self.name + ", and I can bark")
 
    def animal_say_1(self):
        # 子类调用父类的方法
        #  方式1
        super(Dog, self).saySomething()
 
    def animal_say_2(self):
        #  方式2 [推荐]
        super(Dog, self).saySomething()
     
    def animal_say_3(self):
        # 方式3
        Animal.saySomething(self)
 
 
if __name__ == "__main__":
    dog = Dog("Blake")
    dog.saySomething()
    dog.animal_say_1()
    dog.animal_say_2()
    dog.animal_say_3()
    # 子类对象调用被覆盖的父类方法
    super(Dog, dog).saySomething()
