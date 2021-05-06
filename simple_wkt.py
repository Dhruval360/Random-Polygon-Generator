def WKT_writer(arrOfTups):
    txt = ""
    for numTups,tups in enumerate(arrOfTups):
        coorX = tups[0]
        coorY = tups[1]
        txt += str(coorX)
        txt += " "
        txt += str(coorY)
        if numTups != len(arrOfTups)-1:
            txt += ", "
    temp = "POLYGON " + "(" + txt + ")" + "\n"
    f = open("data.txt","w")
    f.write(temp)
    f.close()

exg = [(1,2),(3,4),(5,6)]
WKT_writer(exg)
