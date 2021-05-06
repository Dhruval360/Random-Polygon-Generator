def WKT_writer(LstOfArrOfTups):
    txt = ""
    for counter,arrOfTups in enumerate(LstOfArrOfTups):
        txt += "("
        for numTups,tups in enumerate(arrOfTups):
            coorX = tups[0]
            coorY = tups[1]
            txt += str(coorX)
            txt += " "
            txt += str(coorY)
            if numTups != len(arrOfTups)-1:
                txt += ", "
            else:
                txt += ")"
        if counter != len(LstOfArrOfTups)-1:
            txt += ","
    temp = "POLYGON " + "(" + txt + ")" + "\n"
    f = open("data_comp.txt","w")
    f.write(temp)
    f.close()

exg = [[(1,2),(3,4),(5,6)],[(7,8),(9,10),(11,12)]]
WKT_writer(exg)
