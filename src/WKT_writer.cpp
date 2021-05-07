#include<stdio.h>
#include"Polygon.hpp"

using namespace std;
  
void write(Polygon *p, unsigned num){
    FILE *f = fopen("map.wkt", "w");
    for(unsigned int j = 0; j < num; j++){
        printf("Number of vertices = %u\n", p[j].numVertices);
        fprintf(f, "POLYGON ((");
        for(int i = 0; i < p[j].numVertices; i++) fprintf(f, "%lf %lf, ", p[j].coordinates[i].first, p[j].coordinates[i].second);
        fprintf(f, "%lf %lf))\n", p[j].coordinates[0].first, p[j].coordinates[0].second);
    }
    fclose(f);  
}