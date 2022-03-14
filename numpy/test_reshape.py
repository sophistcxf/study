import numpy as np

def test1():
    a = np.arange(20).reshape(10,2)
    print(a)

    a = np.arange(20).reshape(-1, 10,2)
    print(a)

    print(np.mean(a, axis=0))
    print(np.mean(a, axis=1))
    print(np.mean(a, axis=2))

test1()
