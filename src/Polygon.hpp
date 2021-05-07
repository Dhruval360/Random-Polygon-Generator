#include<stdio.h>
#include<stdlib.h>
#include<utility>
#include<vector>

class Polygon;

void polygonGenerator(double x, double y, double averageRadius, double irregularity, double spike, Polygon *p);
void write(Polygon *p, unsigned num);

class Polygon{
    public:
        unsigned int numVertices;
        //std::pair<double, double> *coordinates;
        std::vector<std::pair<double, double>> coordinates;

        Polygon(){}

        Polygon(unsigned int numVertices){
            this->numVertices = numVertices;
        }

        ~Polygon(){ }

        void Generator1(double x, double y, double averageRadius, double irregularity, double spike){
            polygonGenerator(x, y, averageRadius, irregularity, spike, this);
        };
        void Generator2();
        void Generator3();
};