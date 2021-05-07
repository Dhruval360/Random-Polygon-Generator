#include <fstream>
#include"Polygon.hpp"

using namespace std;
  
void write(Polygon *p, unsigned num){
    ofstream fout;
    fout.open("map.wkt");
    for(unsigned int j = 0; j < num; j++){
        fout << "POLYGON ((";
        for(int i = 0; i < p[j].numVertices; i++) fout << p[j].coordinates[i].first << ' ' << p[j].coordinates[i].second << ", ";
        fout << p[j].coordinates[0].first << ' ' << p[j].coordinates[0].second << "))\n";
    }
    fout.close();  
}