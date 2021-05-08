#include<utility>
#include<vector>

using namespace std;

class Polygon{
    public:
        unsigned int numVertices;
        vector<pair<double, double>> coordinates;

        Polygon(){}

        Polygon(unsigned int numVertices);

        void Generator1(bool verbose);
        void Generator2(bool verbose);
        void Generator3();
}; 

//generates random number between 0 and 1
double random_num();
//initiatres the coordinate vector with num_vertices number of random points
void random_points(Polygon *polygon, int num_vertices, double min, double max);
//return a random point on the given line segment
pair<double,double> random_point_segment(const pair<double,double> &start_point, const pair<double,double> &end_point);
//this function returns a value based on which side the test point is w.r.t the line
bool to_left(const pair<double,double> &start_point, const pair<double,double> &end_point, const pair<double,double> &test_point);
//exchanges contents of the said indices in the vector
void swap(Polygon *polygon, int first_index, int second_index);
//recursive function to solve the problem by the means of divide and conquer
void recursive_partioning(Polygon *polygon, int num_vertices, int begin, int end);
//implementation of the space partition algorithm to generate random polygons
void space_partition(Polygon *polygon, int num_vertices, int min, int max);

void polarGenerator(double x, double y, double averageRadius, double irregularity, double spike, Polygon *p, bool verbose);
void write(Polygon *p, unsigned num); 