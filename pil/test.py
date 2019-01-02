from PIL import Image

if __name__ == "__main__":
    im = Image.open("dog.jpeg", "r")
    print im.size, im.format, im.mode
    im.thumbnail((50, 50), resample=Image.BICUBIC)
    im.show()
