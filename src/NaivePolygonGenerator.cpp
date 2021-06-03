#include <math.h>
#include <iostream>
#include <stack>
#include <math.h>
#include <set>
#include <limits>
#include <algorithm>
#include "Polygon.hpp"
#include "./NaivePoly/Edge.hpp"
#include "./NaivePoly/Helpers.hpp"
#include "./NaivePoly/MyMath.hpp"
#include "./NaivePoly/ConvexHull.hpp"
using namespace std;

double inf = numeric_limits<double>::infinity();
#define col 0
#define cw  1
#define ccw 2

//#define DEBUG  // Uncomment to enable debug

extern double timer;

/******************************* Helper Functions *******************************/
void generatePolygon(Polygon *polygon){
	//vector to store resultant convex hull points
	vector <pair<double,double>> resHull;

	//generate the conve hull points
	generateconvexHull(polygon->coordinates, resHull);

	// Edges of the convex hull
	vector <Edge> edges;

	// Creating edges of convex hull from convex hull vertices
	Edge e1;
	for (int i = 0; i < (int)(resHull.size()-1); ++i){ 
		e1.startVertex = resHull[i];
		e1.endVertex = resHull[i+1];
		edges.push_back(e1);
	}
	// Connect the starting and the last vertex
	e1.startVertex = resHull[resHull.size()-1];
	e1.endVertex = resHull[0];
	edges.push_back(e1);

	// Interior points are those points that are not part of the convex hull vertices
	vector <pair<double,double>> interiorPoints;
	for(auto& v : polygon->coordinates){
		bool isPresent = false;
		for(auto& cv : resHull) 
			if(cv == v){
			 	isPresent = true;
			 	break;
		}
		if(!isPresent) interiorPoints.push_back(v);
	}

	// While there are interior points remaining
	while(interiorPoints.size()){
		// Initially the distance is inf
		double minDist = inf; 		
		Edge toRemEdge;					
		pair<double,double> nearestPoint; 	
		// Iterate over all edges
		for(auto& e : edges){ 				
			// Iterate over all interior points for each edge
			for(auto& ip : interiorPoints){
				// Find distance between line(edge under consideration) and 
				// Point(interior point under consideration)
				double currDist = point2lineDist(ip, e.startVertex, 
														e.endVertex);
				if(currDist < minDist)
					// This can be the minDist between point and edge
					if(e.isValid(edges,ip)){
						minDist = currDist;
						toRemEdge = e;
						nearestPoint = ip;
				}
			}
		}
		// By here we would have the closest point to edge data 
		// The index of the toRemEdge in edges array
		unsigned i = toRemEdge.indexIn(edges);
		
		// Hence replace the "toRemEdge" with an edge from toRemEdge.start
		// to nearestPoint
		Edge e;
		e.startVertex = toRemEdge.startVertex;
		e.endVertex = nearestPoint;
		edges.at(i) = e;
		
		// Insert the edge from nearestPoint to toRemEdge.end
		Edge f;
		f.startVertex = nearestPoint;
		f.endVertex = toRemEdge.endVertex;
		//find iterator of the edge at i+1 and insert the new edge
		auto j = edges.begin()+(i+1);
		edges.insert(j,f);
		//add the point to the current vertices of the polygon ie to the resHull
		//resHull.insert(resHull.begin()+ i,nearestPoint);
		//remove the point from the interior points array
		for(auto i = interiorPoints.begin();i<interiorPoints.end();i++){
			if(*i == nearestPoint){
				interiorPoints.erase(i);
				break;
			}
		}
	}
	// Clear the polygon coordinates of the previous elements
	vector <pair<double,double>> polyPoints;
	for(auto e : edges){
		polyPoints.push_back(e.startVertex);
	}
	polygon->coordinates = polyPoints;
}

/********************************* The Algorithm *********************************/
void naivePolygon(Polygon* polygon, bool verbose){
	start_timer(start);

	generatePolygon(polygon); // Generate the polygon with ordered points
	end_timer(start, timer);
	if(verbose) printf("Number of vertices = %3u | Time taken for generation = %lf s\n",  polygon->numVertices, timer);
}