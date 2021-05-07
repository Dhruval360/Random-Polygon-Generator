import matplotlib.pyplot as plt
from itertools import cycle
cycol = cycle('bgrcmk')
plt.rcParams['axes.facecolor'] = 'gray'
fig = plt.figure()
fig.patch.set_facecolor('orange')

file = open("map.wkt", mode="r")
polygons = file.read().splitlines()
file.close()

def converter(a):
    a = a.split(" ")
    return tuple((float(a[0]), float(a[1])))

for polygon in polygons:
    polygon = polygon.strip("POLYGON")[3:-2:].split(", ")
    polygon = list(map(converter, polygon))
    xCoordinates = [a[0] for a in polygon]
    yCoordinates = [a[1] for a in polygon]
    color = next(cycol)
    for i in range(0, len(xCoordinates)):
        plt.plot(xCoordinates[i:i+2], yCoordinates[i:i+2], c=color,  marker=".", markersize=2, mfc="black", mec='k') 
    #print(polygon)
plt.grid()
plt.show()