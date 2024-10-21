import example

print(example.add(1, 2))

print(example.add2(i=1, j=4))

print(example.add3())

print(example.the_answer)

print(example.what)

p = example.Pet("Molly")
print(p)
print(p.getName())
p.setName("Charly")
print(p.getName())

p = example.Pet2("Sunny")
print(p)
# Pet2不支持动态属性
try:
    p.age = 22
except AttributeError as ex:
    print(ex)

# Pet3支持动态属性
p = example.Pet3("Juddy")
print(p)
p.age = 22
print(p.__dict__)

p = example.Pet4("Jim", 20)
p.set("Jim2")
p.set(10)
print(p.getName())
# 不能直接访问p.name，并不会自动bind属性
try:
    p.name = 30
except AttributeError as ex:
    print(ex)