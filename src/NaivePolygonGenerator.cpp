#include<math.h>
#include<set>
#include<stack>
#include<limits>
#include<iostream>
#include<algorithm>
#include"Polygon.hpp"
using namespace std;

double inf = numeric_limits<double>::infinity();

#define col 0
#define cw  1
#define ccw 2
//#define DEBUG  // Uncomment to enable debug

extern double timer;

// Overloading pair with print operation
ostream& operator << (ostream& out, pair<double,double>& rhs){
	out << "(" << rhs.first << "," << rhs.second << ")";
	return out;
}

// Overloading pair with operator-
pair<double,double> operator - (pair<double,double>& p1, pair<double,double>& p2){
	return {p1.first-p2.first, p1.second-p2.second};
}

struct Edge{
	pair<double,double> startVertex;
	pair<double,double> endVertex;

	bool operator==(struct Edge& rhs){
		return rhs.startVertex == startVertex && rhs.endVertex == endVertex;
	}

	// We use friend to allow rhs as point
	// Or else "this" instance of struct will be used as lhs parameter
	friend ostream& operator << (ostream& out, Edge& rhs){
		out << rhs.startVertex << "->" << rhs.endVertex;
		return out;
	}
};

/******************************* Helper Functions *******************************/

// p is the point
// p1 to p2 is the line segment
double point2lineDist(pair<double,double> &p, pair<double,double> &p1, pair<double,double> &p2){
	// Vectors from p to p1
	double A = p.first - p1.first;
	double B = p.second - p1.second;

	// Line segment vector
	double C = p2.first - p1.first;
	double D = p2.second - p1.second;

	// Dot product of point vec and line segment
	double dot = (A*C)+(B*D);
	// Length of line segment
	double len_seg = (C*C)+(D*D);

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

	double dx = p.first-xx;
	double dy = p.second-yy;
	return (dx*dx)+(dy*dy);
}

/*
	Function to find orientation
	Consider points A, B, C. 
	If slope of A->C is GREATER than slope of A->B,
	then A, B, C are in ccw dir
*/
bool isccw(pair<double,double> &a, pair<double,double> &b, pair<double,double> &c){
	return (c.second-a.second)*(b.first-a.first) > (b.second-a.second)*(c.first-a.first);
}

/*
	Consder the 4 points A, B, C, D. 
	If segment CD intersects segment AB, then ACD and BCD
	will have opposite orientation i.e one will be having 
	clockwise orientation and the other will
	be having anticlockwise orientation. 
	Similarly triangle ABD and ABC will have opposite orientation
*/
bool isIntersectingUtil(pair<double,double> &a, pair<double,double> &b, pair<double,double> &c, pair<double,double> &d){
	return (isccw(a, c, d) != isccw(b, c, d) && isccw(a, b, d) != isccw(a, b, c));
}

// Function to check if the edge e is intersecting with any of the edges of the polygon
bool isIntersectingEdge(vector <Edge> &edges, Edge e){
	// Check for all the edges of the polygon with the edge e for intersection
	for(auto iterE : edges){
		// If edge to be checked and iterEdge share same end points,
		// they can lie on each at max but cannot intersect. 
		// Hence check for that case
		if(e.startVertex != iterE.startVertex && 
			(e.endVertex != iterE.startVertex)){
			if(e.startVertex != iterE.endVertex && 
			e.endVertex != iterE.endVertex){
				// Check for orientation of edge points
				if(isIntersectingUtil(iterE.startVertex,iterE.endVertex,e.startVertex,e.endVertex))
					return true;
			}
		}
	}
	return false;
}

// Function to check if the 2 edges we are creating will be valid or not the edges are valid if they do not intersect the polygon
bool isValidEdge(vector <Edge> &edges, Edge &e, pair<double,double> &p){
	// Create an edge from e.start to the point
	Edge e1;
	e1.startVertex = e.startVertex;
	e1.endVertex = p;

	// Create an edge from p to e.end
	Edge e2;
	e2.startVertex = p;
	e2.endVertex = e.endVertex;

	// Check if the edges are intersecting with the polygon
	if(!isIntersectingEdge(edges, e1) && !isIntersectingEdge(edges, e2)) return true;
	return false;
}

// Function to find index of entity in the vector
int indexInEdgesVec(vector <Edge> arr,Edge k){
	for(int i = 0;i<arr.size();++i)
		if(arr.at(i) == k) return i;

	#ifdef DEBUG
		cout << "\nElement not found\n";
	#endif
    return -1;
}

Edge myFind(vector <Edge> &edges, int &i, bool* status){
	for(int i = 0;i<(int)edges.size();++i)
		if(edges[i] == edges[i]){
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
int orientationOfPoints(pair<double,double> p, pair<double,double> &q, pair<double,double> &r){
	// Find diff between slope
	double resM = (((q.second-p.second)*(r.first-q.first)) - ((q.first-p.first)*(r.second-q.second)));
	#ifdef DEBUG
		cout << "Orientation:" << resM << '\n';
	#endif
	if(resM == 0) return col; // Collinear
	if(resM > 0) return cw;
	return ccw;
}

// Return (euclidsDistance)^2 of the points p1 and p2
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
		while(stk.size()>=2 && orientationOfPoints(justBelowTop(&stk), stk.top(), coordinates[i]) != ccw) stk.pop();
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

void generatePolygon(Polygon *polygon){
	// Vector to store resultant convex hull points
	vector <pair<double,double>> resHull;

	// Generate the conve hull points
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
		for(auto& cv : resHull){
			if(cv == v){
			 	isPresent = true;
			 	break;
			}
		}
		if(!isPresent) interiorPoints.push_back(v);
	}

	// While there are interior points remaining
	while(interiorPoints.size()){
		double minDist = inf; 				// Initially the distance is inf
		Edge toRemEdge;						// Edge to be removed
		pair<double,double> nearestPoint; 	// The nearest point
		for(auto& e : edges){ 				// Iterate over all edges
			for(auto& ip : interiorPoints){ // Iterate over all interior points for each edge
				// Find distance between line(edge under consideration) and 
				// Point(interior point under consideration)
				double currDist = point2lineDist(ip, e.startVertex, 
														e.endVertex);
				if(currDist < minDist)
					if(isValidEdge(edges, e, ip)){ // This can be the minDist between point and edge
						minDist = currDist;
						toRemEdge = e;
						nearestPoint = ip;
				}
			}
		}
		// By here we would have the closest point to edge data 
		// The index of the toRemEdge in edges array
		unsigned i = indexInEdgesVec(edges, toRemEdge);
		// Hence replace the "toRemEdge" with an edge from toRemEdge.start to nearestPoint
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
		// Add the point to the current vertices of the polygon ie to the resHull
		//resHull.insert(resHull.begin()+ i,nearestPoint);
		// Remove the point from the interior points array
		for(auto i = interiorPoints.begin();i<interiorPoints.end();++i){
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
		#ifdef DEBUG
			edgePrinter(e);
			cout << '\n';
		#endif
	}
	#ifdef DEBUG
		cout << "\n\n";
	#endif
	polygon->coordinates = polyPoints;
}

/********************************* The Algorithm *********************************/
void naivePolygon(Polygon* polygon, bool verbose){
	start_timer(start);
	generatePolygon(polygon); // Generate the polygon with ordered points
	end_timer(start, timer);
	if(verbose) printf("Number of vertices = %3u | Time taken for generation = %lf s\n",  polygon->numVertices, timer);
}