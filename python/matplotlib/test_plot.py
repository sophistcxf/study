import matplotlib.pyplot as plt
import numpy

fig, ax = plt.subplots()
x = [[1, 2, 3],[10,20,30]]
y = [[4, 5, 6],[40,50,60]]
ax.scatter(x, y)
ax.plot([1,5,10], [1,7,10], 'k--', lw=4)
ax.plot([1,20,10], [1,7,10], 'k--', lw=4)
x = range(0, 100)
y = numpy.sin(x) * 30
ax.plot(x, y, 'r--', lw=4)
plt.show()
