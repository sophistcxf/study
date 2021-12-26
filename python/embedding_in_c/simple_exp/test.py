import os

print("python pid: ", os.getpid())

if 'a' not in locals().keys():
    a = 0
else:
    a = a + 1
print(a)
