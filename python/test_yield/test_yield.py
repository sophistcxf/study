import traceback

def foo():
  print "yield 1"
  yield 1
  print "yield 2"
  yield 2

p = foo()
print "print p"
print p
print "print p.next() 1"
print p.next()
print "print p.next() 2"
print p.next()
try:
  print "print p.next() 3"
  print p.next()  # exception, StopIteration
except Exception:
  print traceback.print_exc()

# generate object is iterable
p = foo()
for item in p:
  print item


def endless_generate_object():
  number = 0
  while True:
    yield number
    number += 1

p = endless_generate_object()
for i in range(0, 100):
  print p.next() 
  
def generate_obj_send():
  number = 0
  while True:
    val = yield number
    print "hello world"
    if val:
      print "val: ", val
      number = val
    else:
      number += 1

p = generate_obj_send()
print p.next()
print p.next()
# send 10 to generate object
p.send(10)
print p.next()
