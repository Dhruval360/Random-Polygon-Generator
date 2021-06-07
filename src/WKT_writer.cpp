#include <fstream>
#include <iostream>
#include <iomanip>
#include "./Classes/Classes.hpp"

using namespace std;
extern long fileSize;
  
void writer(Polygon *polygons, unsigned num, char *filename){
    ofstream fout;
    if(filename == NULL) fout.open("map.wkt"); // Default value of file name is map.wkt
    else fout.open(filename);
    
    // Traverse through the array of Polygon objects and write to the file 
    for(unsigned int j = 0; j < num; j++){
        if(polygons[j].valid){
            fout << "POLYGON ((";
            // Iterate through all the coordinates in the polygons
            for(unsigned i = 0; i < polygons[j].numVertices; i++) fout << fixed  << polygons[j].coordinates[i].first << ' ' << fixed << polygons[j].coordinates[i].second << ", ";
            // Write the first point as the last point (to complete the polygon)
            fout << polygons[j].coordinates[0].first << ' ' << polygons[j].coordinates[0].second << "))\n";
        }
    }
    fout.seekp(0, ios::end);
    fileSize = fout.tellp();
    fout.close();  
}