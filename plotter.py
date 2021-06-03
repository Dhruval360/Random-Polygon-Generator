import matplotlib.pyplot as plt

def readFile():
	f = open("map.wkt","r")
	lines = f.read()
	#points = []
	f.close()
	#print(lines)
	lines = lines.split("\n")
	numPoly = 0
	for line in lines:
		doesContainDigit = False
		for achar in line:
			if achar.isdigit():
				doesContainDigit = True
				break
		if doesContainDigit:
			X = []
			Y = []
			numPoly += 1
			line = line.replace("POLYGON ","")
			###print(line)
			line = line.replace("(","")
			line = line.replace(")","")
			line = line.replace("\n","")
			#print(line)
			for coord in line.split(", "):
				#print("Coord: {x}".format(x=coord))
				x,y = coord.split(" ")
				X.append(float(x))
				Y.append(float(y))
				#points.append([float(x),float(y)])
				##print("({i},{j})".format(i=x,j=y))
			#create closed loop
			X.append(X[0])
			Y.append(Y[0])
			#points.append(points[0])
			##print(X)
			##print(Y)
			##print(points)
			plt.plot(X,Y,label="POLYGON {cnt}".format(cnt = numPoly))
			del X
			del Y
			#plt.Polygon(points,color="red")
	plt.title("Polygon Plotter")
	plt.legend()
	plt.show()

readFile()
