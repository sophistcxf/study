from sklearn.datasets import load_digits
import matplotlib.pyplot as plt

def test1():
    digits = load_digits()
    print(digits.data.shape)
    plt.gray()
    print "images count: %d" % len(digits.images)
    plt.matshow(digits.images[1])
    plt.show()

if __name__ == '__main__':
    test1()
    pass
