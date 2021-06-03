#include <algorithm>
#include <stack>
#include "ConvexHull.hpp"
#include "MyMath.hpp"
#include "Helpers.hpp"
using namespace std;

#define col 0
#define cw  1
#define ccw 2

void generateconvexHull(vector<pair<double,double>> coordinates,
	vector <pair<double,double>>& resHull){
	// Finding the bottom most point out of all the points
	pair<double,double> min_point = coordinates.at(0);
	int min_index = 0;
	int i = 0;
	for(auto pt : coordinates){
		// Check which is more bottom
		if(pt.second < min_point.second){
			min_point = pt;
			min_index = i;
		}
		// If y coordinate is same, then check for x coordinate
		else if(pt.second == min_point.second && pt.first < min_point.first){
			min_point = pt;
			min_index = i;
		}
		++i;
	}
	// Swap the pos of bottom most point with the first index
	iter_swap(coordinates.begin(), coordinates.begin()+min_index);

	/*
	Sort the remaining n-1 points with respect to this anchor point
	i.e the first point based on the POLAR angle in anti clockwise 
	direction because its a convex hull
	*/
	auto p = coordinates[0];
	qsort_r(&coordinates[1], coordinates.size()-1, sizeof(pair<double,double>), polAngSorter, &p);

	/*
	Now remove all the points with same polar angle from anchor point
	except for the farthest point.
	Since the array is already sorted based on polar angles the 
	farthest point is located at the end
	*/
	for(int i = 1;i<coordinates.size();++i){
		// Iter through the sub vector to find same polar angle points
		int j = i;
		// Count the indices with same polar angles
		while(j < (coordinates.size()-1) && orientationOfPoints(p,coordinates[j], coordinates[j+1]) == col) ++j;
		// Erase from i to j if i!=j
		if(i!=j) coordinates.erase(coordinates.begin() + i, coordinates.begin() + j);
	}

	// If the new vector has less than 3 points, then no convex hull formed
	if(coordinates.size()<3) return ;

	// Create a stack to store the points
	stack <pair<double,double>> stk;

	// Push the first three points p0, p1, p2	
	stk.push(coordinates[0]);	
	stk.push(coordinates[1]);	
	stk.push(coordinates[2]);
	
	// Now start analysing the points from p3
	for(unsigned i = 3; i < coordinates.size(); ++i){
		/*
		Check the orientation i.e. angle formed from the point at the top
		the stack with the curr iter point.
		If its to the right i.e. cw(CONCAVE), 
		then it violates convex hull rule
		*/
		while(stk.size()>=2 && orientationOfPoints(justBelowTop(stk), stk.top(), coordinates[i]) != ccw) stk.pop();
		// Push the next point under consideration onto the stack
		stk.push(coordinates[i]);
	}

	// Stack has all the points of the HULL
	while(!stk.empty()){
		pair<double,double> temp = stk.top();
		resHull.push_back(temp);
		stk.pop();
	}
}