import random

for i in range(0, 10000):
    age = random.randint(0, 50)
    gender = 'male'
    if random.randint(0,2) == 0:
        gender = 'female'
    salary = random.randint(0,1000)
    print '\t'.join(map(str, ['u%s' % i, age, gender, salary]))
