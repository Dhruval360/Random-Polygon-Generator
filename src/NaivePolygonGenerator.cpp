#include<stdlib.h>
#include<stdio.h>
#include<vector>
#include<math.h>
#include<bits/stdc++.h>
#include<stdbool.h>
#include<type_traits>
#include<iostream>
#include<set>
#include<stack>
#include<algorithm>
#include<random>
#include<string>
#include"Polygon.hpp"

#define inf INFINITY
//#define DEBUG  // Uncomment to enable debug
using namespace std;
extern double timer;
extern float Scale;

#define col 0
#define cw  1
#define ccw 2

typedef struct edge{
	pair<double,double> startVertex;
	pair<double,double> endVertex;
}Edge;

/************************************ Globals ************************************/
static pair<double,double> p; // Anchor point to polar sort the rem points

/******************************* Helper Functions *******************************/
// Comparator to insert into set
bool cmpPoints(const pair<double,double> &a,const pair<double,double> &b){
	return ((a.first < b.first) || (a.second < b.second));
}

void randDoubleGen(unsigned numVertices, vector <pair<double,double>> &resHull, vector <pair<double,double>> &allPoints){
	random_device rd; // Random device that initiates a random engine
	mt19937 random_engine(rd()); // The random engine
	uniform_real_distribution<double> unif(-Scale, Scale); // The distribution from which the polygon's vertices are obtained from
	set <pair<double,double>,bool(*)(const pair<double,double>&,const pair<double,double>&)> temp(&cmpPoints); // A set to store non duplicate points only
	for(unsigned i = 0; (unsigned)temp.size() < numVertices; i++){
		double x = unif(random_engine);
		double y = unif(random_engine);
		pair<double,double> p;
		p.first = x;
		p.second = y;
		temp.insert(p);
	}
	for(auto pt : temp)	allPoints.push_back(pt);
}

#ifdef DEBUG
	void pointPrinter(pair<double,double> pt){
		cout << '(' << "X:" << pt.first << ','<< "y:" <<pt.second << ')' << ',' << '\n';
	}

	void edgePrinter(Edge e){
		cout << "From ";
		pointPrinter(e.startVertex);
		cout << "To ";
		pointPrinter(e.endVertex);
	}

	void oriPrinter(int x){
		if(x==col){
			cout << "Collinear\n";
			return;
		}
		if(x==cw) {
			cout << "Clockwise\n";
			return;
		}
		cout << "Anti clockwise\n";
	}
#endif

bool vertexComparator(pair<double,double> a,pair<double,double> b){
	return (a.first == b.first && a.second == b.second);
}

bool vertexNegComparator(pair<double,double> a,pair<double,double> b){
	return (a.first != b.first || a.second != b.second);
}


double point2lineDist(pair<double,double> p, pair<double,double> p1, pair<double,double> p2){
	// Line segment starting point to the point vector
	double A = p.first-p1.first;
	double B = p.second - p1.second;
	
	// Line segment vector
	double C = p2.first - p1.first;
	double D = p2.second - p1.second;

	double dot = (A*C)+(B*D); // Dot product of point vec and line segment
	double len_seg = (C*C)+(D*D); // Length of line segment

	// Cases
	double param = -1;
	if(len_seg !=0) param = ((double)dot/(double)len_seg); // Non zero len line segment
	
	double xx,yy;
	if(param<0){
		xx = p1.first;
		yy = p1.second;
	}
	else if(param >1){
		xx = p2.first;
		yy = p2.second;
	}
	else{
		xx = p1.first + (param*C);
		yy = p1.second + (param*D);
	}

	double dx = p.first-xx;
	double dy = p.second-yy;
	return sqrt((dx*dx)+(dy*dy));
}

/*
	Function to find orientation
	Consider points a,b,c.If slope of A->C is GREATER than slope of A->B,then A,B,c are in ccw dir
*/
bool isccw(pair<double,double> a,pair<double,double> b,pair<double,double> c){
	return (c.second-a.second)*(b.first-a.first) > (b.second-a.second)*(c.first-a.first);
}

/*
	Consder the 4 points A,B,C,D. If segment CD intersects segment AB, then ACD and BCD
	will have opposite orientation i.e one will be having clockwise orientation and the other will
	be having anticlockwise orientation. Similarly triangle ABD and ABC will have opposite orientation
*/
bool isIntersectingUtil(pair<double,double> a,pair<double,double> b,pair<double,double> c,pair<double,double> d){
	//float m1 = (float)(b.second-a.second)/(float)(b.first-a.first);
	//float m2 = (float)(d.second-c.second)/(float)(d.first-c.first);
	////printf("\nThe slopes are %lf and %lf\n",round(m1),round(m2));
	//if(round(m1)==round(m2)) return true;
	return (isccw(a,c,d) != isccw(b,c,d) && isccw(a,b,d) != isccw(a,b,c));
}

// Function to check if the edge e is intersecting with any of the edges of the polygon
bool isIntersectingEdge(vector <Edge> edges,Edge e){
	// Check for all the edges of the polygon with the edge e for intersection
	for(auto iterE : edges){
		// If edge to be checked and iterEdge share same end points, then they can lie on each at max but cannot intersect, hence check for that case
		if(vertexNegComparator(e.startVertex,iterE.startVertex) && 
		vertexNegComparator(e.endVertex,iterE.startVertex)){
			if(vertexNegComparator( e.startVertex ,iterE.endVertex) &&
				vertexNegComparator( e.endVertex,iterE.endVertex)){
				// Check for orientation of edge points
				if(isIntersectingUtil(iterE.startVertex,iterE.endVertex,e.startVertex,e.endVertex)) return true;
			}
		}
	}
	return false;
}

// Function to check if the 2 edges we are creating will be valid or not the edges are valid if they do not intersect the polygon
bool isValidEdge(vector <Edge> edges, Edge e, pair<double, double> p){
	// Create an edge from e.start to the point
	Edge e1;
	e1.startVertex = e.startVertex;
	e1.endVertex = p;
	// Create an edge from p to e.end
	Edge e2;
	e2.startVertex = p;
	e2.endVertex = e.endVertex;
	// Check if the edges are intersecting with the polygon
	if(!isIntersectingEdge(edges,e1) && !isIntersectingEdge(edges,e2)) return true;
	return false;
}

bool edgeComparator(Edge e1,Edge e2){
	return (vertexComparator(e1.startVertex,e2.startVertex) &&
	 vertexComparator(e1.endVertex,e2.endVertex));
}

// Function to find index of entity in the vector
int indexInEdgesVec(vector <Edge> arr, Edge k){ // This needs to be redone
	for(int i = 0; i < (int)arr.size(); i++)
		if(edgeComparator(arr.at(i),k)) return i;
	//#ifdef DEBUG
		cout << "\nElement not found\n";
	//#endif
    return -1;
}

int indexInPointsVec(vector <pair<double,double>> arr,pair<double,double> k){
	for(int i = 0;i<(int)arr.size();i++)
		if(vertexComparator(arr.at(i),k)) return i;
	#ifdef DEBUG
		cout << "\nElement not found\n";
	#endif
    return -1;
}

Edge myFind(vector <Edge> edges,int i,bool* status){
	for(int i = 0;i<(int)edges.size();i++)
		if(edgeComparator(edges[i], edges[i])){
			*status = true;
			return edges[i];
		}
	#ifdef DEBUG
		cout << "\nNo edge found!\n";
	#endif
	*status = false;
	return edges[0];
}



// Orientation of 3 pints depends on their slope
// If diff of slopes is positive, then there is RIGHT turn i.e cw else if negative then ccw i.e LEFT or else collinear if 0
int orientationOfPoints(pair<double,double> p,pair<double,double> q,pair<double,double> r){
	// Find diff between slope
	double resM = (((q.second-p.second)*(r.first-q.first)) - ((q.first-p.first)*(r.second-q.second)));
	#ifdef DEBUG
		cout << "Orientation:" << resM << '\n';
	#endif
	if(resM == 0) return col; // Collinear
	if(resM>0) return cw;
	return ccw;
}

// Return (euclidsDistance)^2 of the points p1 and p2
double euclidsDist(pair<double,double> p1,pair<double,double> p2){
	return ((p2.first-p1.first)*(p2.first-p1.first))+((p2.second-p1.second)*(p2.second-p1.second));
}

// Sorting based on polar angles from the anchor point
//int polAngSorter(pair<double,double> p1,pair<double,double> p2){
int polAngSorter(const void* vp1, const void* vp2){
	pair<double,double> p1 = *((pair<double,double>*)vp1);
	pair<double,double> p2 = *((pair<double,double>*)vp2);
	int ori = orientationOfPoints(p,p1,p2);
	switch(ori){
		// If they are collinear then use euclids distance
		case col:
			if(euclidsDist(p,p2)>=euclidsDist(p,p1)) return -1;
			return 1;

		case cw:
			return 1;

		case ccw:
			return -1;

		default:
			printf("\nUnable to sort?\n");
		return 0;
	}
}

// Get the element just below the top
pair<double,double> justBelowTop(stack <pair<double,double>>* stk){
	pair<double,double> theTop = stk->top();
	stk->pop();
	pair<double,double> retVal = stk->top();
	stk->push(theTop);
	return retVal;
}

void generateconvexHull(vector <pair<double,double>> points, vector <pair<double,double>> &resHull){
	// Finding the bottom most point out of all the points
	pair<double,double> min_point = points.at(0);
	int min_index = 0;
	int i = 0;
	for(auto pt : points){
		// Check which is more bottom
		if(pt.second<min_point.second){
			min_point = pt;
			min_index = i;
		}
		// If y coord is same the check for x coordinate
		else if(pt.second == min_point.second && pt.first < min_point.first){
			min_point = pt;
			min_index = i;
		}
		i++;
	}
	#ifdef DEBUG
		cout<<"Bottom most point : \n";
		pointPrinter(min_point);
	#endif

	// Swap the pos of bottom most point with the first index
	iter_swap(points.begin()+0,points.begin()+min_index);
	
	#ifdef DEBUG
		cout << "After swapping : \n";
		for(int i = 0;i<(int)points.size();i++) pointPrinter(points[i]);
	#endif
	// Sort the remaining n-1 points with respect to this anchor point
	// i.e the first point based on the POLAR angle in anti clock wise direction because its a convex hull
	p = points[0];
	//SORT does NOT work,fix that - maybe we need to send boolean for SORT
	//sort(points.begin()+1,points.end(),polAngSorter);
	qsort(&points[1],points.size()-1, sizeof(pair<double,double>), polAngSorter);

	#ifdef DEBUG
		for(int i = 0;i<(int)points.size();i++) pointPrinter(points[i]);
	#endif
	// Now remove all the points with same polar angle from anchor point except for the farthest point
	// The array is already sorted based on polar angles and also when the polar angles where same we consdered their dist from p0 as the  factor for sorting.
	// Hence the farthest point with similar polar angles is located at the end
	for(int i = 1;i<(int)points.size();i++){ // Iterate through the sub vector to find same polar angle points
		int j = i;
		// Count the indices with same polar angles
		while(j< (int)(points.size()-1) && 
			orientationOfPoints(p,points[j],points[j+1]) == col){
			j++;
		}
		if(i!=j){ // Erase from i to j if i!=j
			#ifdef DEBUG
				cout << "i "<< i << '\n';
				cout << "j "<< j << '\n';
				cout << j-i << " Dups found,removing them..\n";
			#endif
			points.erase(points.begin()+i,points.begin()+(j));
		}
	}

	#ifdef DEBUG
		cout << "After removing duplicates : \n";
		for(int i = 0;i<(int)points.size();i++)	pointPrinter(points[i]);
	#endif

	// If the new vec has LT 3 points,then no convex hull formed
	if(points.size()<3) return ;

	// Create the stack to store the points
	// Push the first three points p0,p1,p2
	stack <pair<double,double>> stk;
	#ifdef DEBUG
		cout << "Pushing.....\n";
		pointPrinter(points[0]);
		pointPrinter(points[1]);
		pointPrinter(points[2]);
	#endif
	stk.push(points[0]);
	stk.push(points[1]);
	stk.push(points[2]);
	/*
	|p2|
	|p1|
	|p0|
	----
	*/

	// Now start analysing the points from p3
	for(int i = 3; i<(int)points.size(); i++){
		// Check the orientation i.e angle formed from the point at the top of the stack with the curr iter point.
		// If its to the right i.e cw(CONCAVE), then violates convex hull rule
		#ifdef DEBUG
			pointPrinter(points[i]);
		#endif
		while(stk.size()>1 && orientationOfPoints(justBelowTop(&stk), stk.top(), points[i]) != ccw) stk.pop();
		stk.push(points[i]); // Push the next point under consideration
	}
	#ifdef DEBUG
			cout << "stk size:" << stk.size() <<' \n';
			cout << "points arr size " << points.size() << '\n';
			cout << "Result : \n";
	#endif
	

	// Stack has all the points of the HULL
	while(!stk.empty()){
		pair<double,double> temp = stk.top();
		#ifdef DEBUG
			cout<<"Result : \n";
			pointPrinter(temp);
		#endif
		resHull.push_back(temp);
		stk.pop();
	}
}

void generatePolygon(Polygon *polygon){
	vector <pair<double,double>> resHull; // The resultant CONVEX HULL
	vector <pair<double,double>> allPoints; // All points needed to be included in POLYGON - Supplied as random points
	vector <pair<double,double>> polyPoints; // The result points
	resHull.reserve(polygon->numVertices);
	allPoints.reserve(polygon->numVertices);
	polyPoints.reserve(polygon->numVertices);
	randDoubleGen(polygon->numVertices, resHull, allPoints);
	generateconvexHull(allPoints, resHull);
	// Points in the convex Hull
	// vector <pair<double,double>> resHull = {{-5,-3},{-1,1},{0,0},{1,-4},{-1,-5}};
    // vector <pair<double,double>> resHull = {{0,0},{0,3},{3,1},{4,4}};              
	// resHull = util(allPoints);
	// In real, get from utils function

	// Edges of the convex hull
	vector <Edge> edges;
	// Creating edges of poly from convex hull vertices
	for (int i = 0; i < (int)(resHull.size()-1); ++i){
		Edge e1;
		e1.startVertex = resHull[i];
		e1.endVertex = resHull[i+1];
		edges.push_back(e1);
	}
	// Connect the starting and the last vertex
	Edge e;
	e.startVertex = resHull[resHull.size()-1];
	e.endVertex = resHull[0];
	edges.push_back(e);

	// Interior points are those points that are not part of the convex hull vertices
	// Interior points are correctly generated
	vector <pair<double,double>> interiorPoints;
	for(auto v : allPoints){
		bool isPresent = false;
		for(auto cv : resHull)
			if(cv.first == v.first && cv.second == v.second) isPresent = true;
		
		if(!isPresent) interiorPoints.push_back(v);
	}

	#ifdef DEBUG
		cout << "Interior Points : ";
		for(auto pt : interiorPoints) pointPrinter(pt);
		cout << "\nThe edges of convex Hull are : \n";
		for(auto e : edges) edgePrinter(e);
	#endif

	// While there are interior points remaining
	while(interiorPoints.size()> 0){
		double minDist = inf; // Initially the distance is infinite
		Edge toRemEdge; // Edge to be removed
		pair<double,double> nearestPoint; // The nearest point
		for(auto e : edges){ // Iterate over all edges
			for(auto ip : interiorPoints){ // Iterate over all interior points for each edge
				// Find distance between line(edge under consideration) and point(interior point under consideration)
				double currDist = point2lineDist(ip,e.startVertex,e.endVertex);
				#ifdef DEBUG
					cout << "Curr Dist " << currDist << '\n';
					cout<<"Dist of edge ";edgePrinter(e);//cout<<" To point ";
					pointPrinter(ip);
					cout<<" is " <<currDist << '\n';
				#endif
				if(currDist < minDist)
					if(isValidEdge(edges,e,ip)){ // This can be the minDist between point and edge
						minDist = currDist;
						toRemEdge = e;
						nearestPoint = ip;
				}
			}
		}
		
		#ifdef DEBUG
			cout <<"Choosen : \n";
			cout<<"Dist of edge ";edgePrinter(toRemEdge);
			cout<<" To point ";
			pointPrinter(nearestPoint);
			cout<<" is " << minDist << '\n';
			cout<<"End\n";
		#endif
		// Here we would have the closest point to edge data and the index of the toRemEdge in edges array
		int i = indexInEdgesVec(edges, toRemEdge);
		// Hence replace the "toRemEdge" with an edge from toRemEdge.start to nearestPoint
		Edge e;
		e.startVertex = toRemEdge.startVertex;
		e.endVertex = nearestPoint;
//cout << i << '\n'; // Code breaks here when i is -1. i.e toRemEdge is not found in the edges vector
		edges.at(i) = e;
		// Insert the edge from nearestPoint to toRemEdge.end
		Edge f;
		f.startVertex = nearestPoint;
		f.endVertex = toRemEdge.endVertex;
		// Find iterator of the edge at i+1 and insert the new edge
		/*
			//auto j = find(edges.begin(),edges.end(),edges.at(i+1));
			//bool isFindSuccess = false;
			//auto j = myFind(edges,i+1,&isFindSuccess);
		*/
		auto j = edges.begin()+(i+1);
		edges.insert(j,f);
		// Add the point to the current vertices of the polygon i.e to the resHull
		//resHull.insert(resHull.begin()+ i,nearestPoint);
		// Remove the point from the interior points array
		for(auto i = interiorPoints.begin(); i<interiorPoints.end(); i++){
			if(vertexComparator(*i, nearestPoint)){
				interiorPoints.erase(i);
				break;
			}
		}
	}
	for(auto e : edges){
		polygon->coordinates.push_back(e.startVertex);
		#ifdef DEBUG
			edgePrinter(e);
		#endif
	}
	//polygon->coordinates = polyPoints;
	//allPoints.clear();
	//polyPoints.clear();
	//resHull.clear();
}

/********************************* The Algorithm *********************************/
void naivePolygon(Polygon* polygon,bool verbose){
	start_timer(start);
	generatePolygon(polygon);    // Generate the polygon with ordered points
	end_timer(start, timer);
	if(verbose) printf("Number of vertices = %3u | Time taken for generation = %lf s\n",  polygon->numVertices, timer);
}