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
        float color[3]; // Holds the rgb color values for the polygon's edges
        vector<pair<double, double>> coordinates;

        Polygon(){}

        Polygon(unsigned int numVertices);

        void Generator1(bool verbose, int choice);
        void Generator2(bool verbose, int choice);
        void Generator3(bool verbose, int choice);
}; 


void spacePartition(Polygon *polygon, int min, int max, bool verbose);
void naivePolygon(Polygon *polygon, bool verbose);
void polarGenerator(double x, double y, double averageRadius, double irregularity, double spike, Polygon *p, bool verbose);
void writer(Polygon *p, unsigned num, char *filename); 
void* GraphicsInit(void *arg);