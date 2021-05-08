#include<fstream>
#include<iostream>
#include<iomanip>
#include"Polygon.hpp"

using namespace std;
  
void write(Polygon *polygons, unsigned num, char *filename){
    ofstream fout;
    //default value of file name is map.wkt
    if(filename == NULL){
        fout.open("map.wkt");
    }
    else{
        fout.open(filename);
    }
    //traverse through the polygons class object and write to the file
    //iterate through all the polygons
    for(unsigned int j = 0; j < num; j++){
        fout << "POLYGON ((";
        //iterate through all the co-ordinates in the polygons
        for(unsigned i = 0; i < polygons[j].numVertices; i++) fout << fixed  << polygons[j].coordinates[i].first << ' ' << fixed << polygons[j].coordinates[i].second << ", ";
        //write the first point as the last point also
        fout << polygons[j].coordinates[0].first << ' ' << polygons[j].coordinates[0].second << "))\n";
    }
    fout.seekp(0, ios::end);
    cout << "Done\nFiles size = " << fout.tellp() << " B\n";
    fout.close();  
}