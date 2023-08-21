import os

res = os.popen('ls -l')
for l in res.readlines():
    print(l.strip())
