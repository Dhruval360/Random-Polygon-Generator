#include <utility>

using namespace std;

#define col 0 // Colenear
#define cw  1 // Clockwise
#define ccw 2 // Counter Clockwise

/*
	Function to find orientation
	Consider points A, B, C. 
	If slope of A->C is GREATER than slope of A->B,
	then A, B, C are in ccw dir
*/
bool isccw(pair<double,double> &a,pair<double,double> &b, pair<double,double> &c);


// p is the point
// p1 to p2 is the line segment
double point2lineDist(pair<double,double> &p,
 pair<double,double> &p1, pair<double,double> &p2);

/* 
	Orientation of 3 pints depends on their slope.
	If diff of slopes is positive, then there is RIGHT turn i.e cw
	else if negative then ccw i.e LEFT or else collinear if 0
*/
int orientationOfPoints(pair<double,double> p,
 pair<double,double> &q, pair<double,double> &r);

// Return (euclidsDistance)^2 of the points p1 and p2
double euclidsDist(pair<double,double> &p1, pair<double,double> &p2);

// Sorting based on polar angles from the anchor point
int polAngSorter(const void* vp1, const void* vp2, void* anchor);