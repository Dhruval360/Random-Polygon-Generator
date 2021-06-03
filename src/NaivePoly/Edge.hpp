#include <iostream>
#include <vector>
using namespace std;

ostream& operator<<(ostream& out,pair<double,double>& rhs);
pair<double,double> operator-(pair<double,double>& p1,
	pair<double,double>& p2);

class Edge{
	public:
	pair<double,double> startVertex;
	pair<double,double> endVertex;

	//constructor
	Edge(pair<double,double>p1 = {-1,-1}, pair<double,double> p2 = {-1,-1});

    //friend func operator overload for == operator
	friend bool operator==(Edge& rhs,Edge& lhs);

	//we use friend to allow rhs as point
	//or else "this" instance of struct will be used as lhs parameter
	friend ostream& operator<<(ostream& out,Edge& rhs);

	//function to find index of entity in the vector
	int indexIn(vector <Edge>& arr);

	/*
	Consder the 4 points A, B, C, D. 
	If segment CD intersects segment AB, then ACD and BCD
	will have opposite orientation i.e one will be having 
	clockwise orientation and the other will
	be having anticlockwise orientation. 
	Similarly triangle ABD and ABC will have opposite orientation
	*/
	bool intersectingUtil(pair<double,double> &a,pair<double,double> &b);

	/*Function to check if the edge e is 
	intersecting with any of the exisitng polygon edges*/
	bool isIntersecting(vector <Edge> &edges);

	/* Function to check if the 2 edges we 
	are creating will be valid or not the edges are 
	valid if they do not intersect the polygon*/
	bool isValid(vector <Edge> &edges, pair<double,double> &p);
};