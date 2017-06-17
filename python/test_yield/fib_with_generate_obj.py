def fib():
  a, b = 0, 1
  while True:
    yield a
    a, b = b, a+b


p = fib()
for i in range(1, 100):
  print p.next(), ",",
