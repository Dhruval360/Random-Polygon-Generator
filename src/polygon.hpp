#include<stdio.h>
#include<stdlib.h>
#include<utility>
#include<vector>
//***********************Dhruval***********************
class Polygon{
    public:
        unsigned int numVertices;
        std::vector<std::pair<double, double>> coordinates;

        Polygon(){}

        Polygon(unsigned int numVertices){
            this->numVertices = numVertices;
            //this->coordinates = (std::pair<double, double> *)malloc(sizeof(std::pair<double, double>) * numVertices);
        }

        ~Polygon(){
        }
};
double clip(double x, double min, double max);
void polygonGenerator(double x, double y, double averageRadius, double irregularity, double spike, Polygon &p);
void write(Polygon *p, unsigned num);
/*
//***********************space partition***********************
//structure that represents a point in the polygon
typedef struct point_structure{
	double x;
	double y;
	point_structure() : x(0.0), y(0.0) {}
	point_structure(double xp, double yp) : x(xp), y(yp){}
}point;
//generates a random number between 0 and 1
double randf();
//generates random points
void random_points(point points_array[], int num_vertices, double min, double max);
//returns a random point on the line segment
point random_point_on_segment(const point &s, const point &e);
bool to_left(const point &s, const point &e, const point &p);
//exchanges contents of two indices in the array
void swap(point points_array[], int i, int j);
//implementation of the space partition algorithm to generate random polygons
void space_partition(point points_array[], int num_vertices);
//logic for partitioning the array into two disjoint convex hulls
void partion_logic(point points_array[], int temp_lr, int temp_rl, int begin, int end, point rps, point rpe, bool flag);
//function to recursively solve the algorithm
void recursive_partitioning(point points_array[], int num_vertices, int l, int r);
//writes the randomly generated points to the wkt file
void write_to_wkt(point points_array[], int num_vertices, char *filename);*/