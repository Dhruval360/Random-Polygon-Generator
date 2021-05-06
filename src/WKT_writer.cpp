#include<stdio.h>
#include"polygon.hpp"

using namespace std;
  
void write(Polygon *p, unsigned num){
    FILE *f = fopen("WKT.txt", "w");
    for(unsigned int j = 0; j < num; j++){
        printf("Number of vertices = %u\n", p[j].numVertices);
        fprintf(f, "POLYGON ((");
        for(int i = 0; i < p[j].numVertices; i++) fprintf(f, "%lf %lf, ", p[j].xCoordinates[i], p[j].yCoordinates[i]);
        fseek(f, -2, SEEK_CUR);
        fprintf(f, "))\n");
    }
    fclose(f);  
}