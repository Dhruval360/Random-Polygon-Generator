#include <fstream>
#include <iostream>
#include <iomanip>
#include <ctime>
#include "./Classes/Classes.hpp"

using namespace std;

#define WAIT 50000000L
extern long fileSize;
extern Polygon *polygons;
extern char *filename;
  
void *writer(void *arg) {
    unsigned *num = (unsigned*)arg;
    ofstream fout;
    if(filename == NULL) {
        fout.open("map.wkt"); // Default value of file name is map.wkt
        cout << "Opened map.wkt\n";
    }
    else fout.open(filename);
    // Traverse through the array of Polygon objects and write to the file 
    for(unsigned int j = 0; j < *num; j++) {
        while(polygons[j].processing) nanosleep((const struct timespec[]){{0, WAIT}}, NULL);
        if(polygons[j].valid) {
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
    return NULL;
}