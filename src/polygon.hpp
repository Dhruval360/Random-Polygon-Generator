#include<stdio.h>
#include<stdlib.h>
#include<utility>


class Polygon{
    public:
        unsigned int numVertices;
        std::pair<double, double> *coordinates;

        Polygon(){}

        Polygon(unsigned int numVertices){
            this->numVertices = numVertices;
            this->coordinates = (std::pair<double, double> *)malloc(sizeof(std::pair<double, double>) * numVertices);
        }

        ~Polygon(){
            free(this->coordinates);
        }
};

void write(Polygon *p, unsigned num);