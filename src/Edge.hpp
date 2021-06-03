#include <iostream>
using namespace std;

struct Edge{
	pair<double,double> startVertex;
	pair<double,double> endVertex;

    //friend func operator overload for == operator
	friend bool operator==(struct Edge& rhs,struct Edge& lhs);

	//we use friend to allow rhs as point
	//or else "this" instance of struct will be used as lhs parameter
	friend ostream& operator<<(ostream& out,Edge& rhs);
};