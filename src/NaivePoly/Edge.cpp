#include <iostream>
#include <vector>
#include "Edge.hpp"
#include "MyMath.hpp"
using namespace std;

/**************OverLoaded Operator Functions************************/
//overloading pair with print operation
ostream& operator<<(ostream& out,pair<double,double>& rhs){
	out << "(" << rhs.first << "," << rhs.second << ")";
	return out;
}

//overloading pair with operator-
pair<double,double> operator-(pair<double,double>& p1,
	pair<double,double>& p2){
	return {p1.first-p2.first,p1.second-p2.second};
}

bool operator==(Edge& lhs,Edge& rhs){
    return rhs.startVertex == lhs.startVertex 
	&& rhs.endVertex == lhs.endVertex;
}

ostream& operator<<(ostream& out,Edge& rhs){
		out << rhs.startVertex << "->" << rhs.endVertex;
		return out;
}

Edge::Edge(pair<double,double> p1, pair<double,double> p2){
	this->startVertex = p1;
	this->endVertex = p2;
}

int Edge::indexIn(vector <Edge>& arr){
	for(int i = 0;i<arr.size();i++){
		if(arr.at(i) == *this){
			return i;
		}
	}
    return -1;
}

bool Edge::intersectingUtil(pair<double,double> &a,
    pair<double,double> &b){
		//alias for start and end vertex
		auto& c = this->startVertex;
		auto& d = this->endVertex;
        return (isccw(a, c, d) != isccw(b, c, d) &&
        isccw(a, b, d) != isccw(a, b, c));
}


bool Edge::isIntersecting(vector <Edge> &edges){
    /* Check for all the edges of the polygon with the edge e
    for intersection */
    for(Edge iterE : edges){
        /*if edge to be checked and iterEdge share same end points,
        then they can lie on each
        at max but cannot intersect,
        hence check for that case*/
        if(iterE.startVertex == this->startVertex || 
        iterE.startVertex == this->endVertex ||
        iterE.endVertex == this->startVertex ||
        iterE.endVertex == this->endVertex) continue;
        
        //check for orientation of edge points
		if(this->intersectingUtil(iterE.startVertex,iterE.endVertex))
            return true;
    }
    return false;
}


bool Edge::isValid(vector <Edge>& edges, pair<double,double>& p){
	//create an edge from e.start to the point
	Edge e1(this->startVertex,p);

	//create an edge from p to e.end
	Edge e2(p,this->endVertex);

	//check if the edges are intersecting with the polygon
	if(!e1.isIntersecting(edges) &&
     !e2.isIntersecting(edges)) return true;
	return false;
}