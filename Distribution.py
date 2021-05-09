import matplotlib.pyplot as plt
from itertools import cycle
from statistics import mean
import scipy.stats
import sys

cycol = cycle('bgrcmk')
plt.rcParams['axes.facecolor'] = 'lightgrey'

fig = plt.figure()
fig.patch.set_facecolor('gray')

if len(sys.argv) > 1:
    file = open(sys.argv[1], mode = "r")
else:
    file = open("map.wkt", mode = "r")

polygons = file.read().splitlines()
file.close()

x_avg_list, y_avg_list, distance = [], [], []

def distance_from_origin(x, y):
    for i in range(len(x)):
        distance.append((x[i]**2 + y[i]**2)**(0.5))

def converter(a):
    a = a.split(" ")
    return tuple((float(a[0]), float(a[1])))

for polygon in polygons:
    polygon = list(map(converter, polygon.strip("POLYGON")[3:-2:].split(", ")))
    xCoordinates, yCoordinates = [a[0] for a in polygon], [a[1] for a in polygon]
    color = next(cycol)
    x_avg, y_avg = mean(xCoordinates), mean(yCoordinates)
    x_avg_list.append(x_avg)
    y_avg_list.append(y_avg)

#print(x_avg_list)
#print(y_avg_list)
distance_from_origin(x_avg_list, y_avg_list)
_, bins, _ = plt.hist(distance, 20, density=1, alpha=0.5)
mu, sigma = scipy.stats.norm.fit(distance)
best_fit_line = scipy.stats.norm.pdf(bins, mu, sigma)
plt.plot(bins, best_fit_line)
plt.show()