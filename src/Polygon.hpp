#include<vector>
#include "NaivePoly/Edge.hpp"
#include<chrono>

using namespace std;

#define start_timer(start) auto start = chrono::high_resolution_clock::now();  

#define end_timer(start, var)\
  double time_taken =  chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start).count();\
  time_taken *= 1e-9;\
  var = time_taken;  

class Polygon{
    public:
        unsigned int numVertices;
        float color[3]; // Holds the rgb color values for the polygon's edges
        vector<pair<double, double>> coordinates;
        //vector to store the edges of the final polygon
        vector<Edge> edges;

        Polygon(){}

        Polygon(unsigned int numVertices);

        //function to check if the polygon formed are intersection free
        bool validityCheck();
        //function to convert points of polygon to edges of the poly
        void constructEdges();

        void Generator1(bool verbose, int choice);
        void Generator2(bool verbose, int choice);
        void Generator3(bool verbose, int choice);
}; 

void spacePartition(Polygon *polygon, bool verbose);
void naivePolygon(Polygon *polygon, bool verbose);
void polarGenerator(double x, double y, double averageRadius, double irregularity, double spike, Polygon *p, bool verbose);
double clip(double x, double min, double max);
void writer(Polygon *p, unsigned num, char *filename); 
void* GraphicsInit(void *arg);