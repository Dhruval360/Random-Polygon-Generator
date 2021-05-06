#include <stdio.h>
#include<stdlib.h>


class Polygon{
    public:
        unsigned int numVertices;
        double *xCoordinates, *yCoordinates;

        Polygon(){}

        Polygon(unsigned int numVertices){
            this->numVertices = numVertices;
            this->xCoordinates = (double*)malloc(sizeof(double) * numVertices);
            this->yCoordinates = (double*)malloc(sizeof(double) * numVertices);
        }

        ~Polygon(){
            free(this->xCoordinates);
            free(this->yCoordinates);
        }
};

void write(Polygon *p, unsigned num);