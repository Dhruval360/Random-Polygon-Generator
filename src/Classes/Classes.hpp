#include <vector>
#include <chrono>
#include <iostream>

using namespace std;

#define start_timer(start) auto start = chrono::high_resolution_clock::now();  

#define end_timer(start, var)\
  double time_taken =  chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start).count();\
  time_taken *= 1e-9;\
  var = time_taken;  

class Edge{
	public:
		pair<double,double> startVertex;
		pair<double,double> endVertex;

		Edge(pair<double,double> p1 = {-1, -1}, pair<double,double> p2 = {-1, -1});

		// Friend function operator overload for == operator
		friend bool operator==(Edge& lhs,Edge& rhs);

		// We use friend to allow rhs as point or else "this" instance of struct will be used as lhs parameter
		friend ostream& operator<<(ostream& out, Edge& rhs);

		// Function to find index of entity in the vector
		int indexIn(vector <Edge>& arr);

		/*
		Consder the 4 points A, B, C, D. 
		If segment CD intersects segment AB, then ACD and BCD will have opposite orientation 
		i.e one will be having clockwise orientation and the other will be having anticlockwise orientation. 
		Similarly, triangle ABD and ABC will have opposite orientation
		*/
		bool intersectingUtil(pair<double,double> &a,pair<double,double> &b);

		
		// Function to check if the edge e is intersecting with any of the exisitng polygon edges
		bool isIntersecting(vector <Edge> &edges, bool log=false);

		/* 
		Function to check if the 2 edges we are creating will be valid or not.
		Edges are  valid if they do not intersect the polygon
		*/
		bool isValidEdge(vector <Edge> &edges, pair<double,double> &p);
};

class Polygon{
    public:
        bool valid = true;                        // Holds the validity of the polygon
        unsigned int numVertices;
        float color[3];                           // Holds the rgb color values for the polygon's edges
        vector<pair<double,double>> coordinates;
        vector<Edge> edges;                       // Vector to store the edges of the final polygon

        Polygon(){}

        Polygon(unsigned int numVertices);

        bool validityCheck();  // Checks if the polygon formed are intersection free
        void constructEdges(); // Construct the egdes of the polygon from its vertices

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
ostream& operator << (ostream& out,pair<double,double>& rhs);
pair<double,double> operator - (pair<double,double>& p1, pair<double,double>& p2);