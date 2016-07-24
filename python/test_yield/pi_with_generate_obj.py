def pi():
  total = 0
  i = 1.0
  j = 1
  while True:
    total = total + j / i
    yield 4 * total
    i += 2
    j = j * -1

def main(g, n):
  for i in range(n):
    yield g.next()

if __name__ == "__main__":
  print list(main(pi(), 8))
