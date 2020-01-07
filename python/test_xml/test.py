from xml import sax

f = open('sample.xml', 'r')

lines = f.readlines()

f.close()

print lines

for l in lines:
    sax.parseString(l)

