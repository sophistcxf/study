def foo(x, y, *z, **m):
  print x
  print y
  print z
  print m

foo(1, 2, 3, 4, 5, 6, 7, m=8, n=9, t=10)
