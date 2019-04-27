import random

def seed():
    return 0.6

def test_shuffle():
    l1 = range(1000)
    random.shuffle(l1, seed)
    print l1[0:10]
    l2 = range(1000)
    random.shuffle(l2, seed)
    print l2[0:10]
    if l1 == l2:
        print "equal"

if __name__ == "__main__":
    test_shuffle()
