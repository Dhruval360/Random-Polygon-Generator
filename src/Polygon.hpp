#include<utility>
#include<vector>
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
        vector<pair<double, double>> coordinates;

        Polygon(){}

        Polygon(unsigned int numVertices);

        void Generator1(bool verbose);
        void Generator2(bool verbose);
        void Generator3(bool verbose);
}; 

//implementation of the space partition algorithm to generate random polygons
void space_partition(Polygon *polygon, int min, int max, bool verbose);

void polarGenerator(double x, double y, double averageRadius, double irregularity, double spike, Polygon *p, bool verbose);
void writer(Polygon *p, unsigned num, char *filename); 
void GraphicsInit();