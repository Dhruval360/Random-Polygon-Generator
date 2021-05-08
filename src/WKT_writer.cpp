#include <fstream>
#include"Polygon.hpp"
#include<iostream>
#include<iomanip>
using namespace std;
  
void write(Polygon *polygon, unsigned num, char *filename){
    ofstream fout;
    //default value of file name is map.wkt
    if(filename == NULL){
        fout.open("map.wkt");
    }
    else{
        fout.open(filename);
    }
    //traverse through the polygon class object and write to the file
    //iterate through all the polygons
    for(unsigned int j = 0; j < num; j++){
        fout << "POLYGON ((";
        //iterate through all the co-ordinates in the polygon
        for(int i = 0; i < polygon[j].numVertices; i++) fout << fixed  << polygon[j].coordinates[i].first << ' ' << fixed << polygon[j].coordinates[i].second << ", ";
        //write the first point as the last point also
        fout << polygon[j].coordinates[0].first << ' ' << polygon[j].coordinates[0].second << "))\n";
    }
    fout.seekp(0, ios::end);
    cout << "Done\nFiles size = " << fout.tellp() << " B\n";
    fout.close();  
}