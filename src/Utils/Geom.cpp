#include <utility>
#include <iostream>
#include "Geom.hpp"

using namespace std;

bool isccw(pair<double,double> &a,pair<double,double> &b, pair<double,double> &c){
	return (c.second-a.second)*(b.first-a.first) > (b.second-a.second)*(c.first-a.first);
}

double point2lineDist(pair<double,double> &p, pair<double,double> &p1, pair<double,double> &p2){
	// Vectors from p to p1
	double A = p.first - p1.first;
	double B = p.second - p1.second;

	// Line segment vector
	double C = p2.first - p1.first;
	double D = p2.second - p1.second;

	// Dot product of point vec and line segment
	double dot = (A*C) + (B*D);
	// Length of line segment
	double len_seg = (C*C) + (D*D);

	// Cases
	double param = -1;
	if(len_seg != 0) param = (dot/len_seg); // Line segment of non zero length
	
	double xx, yy;
	if(param < 0){
		xx = p1.first;
		yy = p1.second;
	}
	else if(param > 1){
		xx = p2.first;
		yy = p2.second;
	}
	else{
		xx = p1.first + (param*C);
		yy = p1.second + (param*D);
	}

	double dx = p.first  - xx;
	double dy = p.second - yy;
	return (dx*dx) + (dy*dy);
}

int orientationOfPoints(pair<double,double> p, pair<double,double> &q, pair<double,double> &r){
	// Find diff between slope
	double resM = (((q.second-p.second)*(r.first-q.first)) - ((q.first-p.first)*(r.second-q.second)));
	if(resM == 0) return col; // Collinear
	if(resM > 0) return cw;
	return ccw;
}

double euclidsDist(pair<double,double> &p1, pair<double,double> &p2){
	return ((p2.first-p1.first)*(p2.first-p1.first)) + ((p2.second-p1.second)*(p2.second-p1.second));
}

// Sorting based on polar angles from the anchor point
int polAngSorter(const void* vp1, const void* vp2, void* anchor){
	pair<double,double> p1 = *((pair<double,double>*)vp1);
	pair<double,double> p2 = *((pair<double,double>*)vp2);
	pair<double,double> p  = *((pair<double,double>*)anchor);
	int ori = orientationOfPoints(p, p1, p2);
	switch(ori){
		// If they are collinear then use euclids distance
		case col:
			if(euclidsDist(p, p2)>=euclidsDist(p, p1)) return -1;
			return 1;

		case cw:
			return 1;

		case ccw:
			return -1;

		default:
			cout << "\nUnable to sort\n";
		return 0;
	}
}