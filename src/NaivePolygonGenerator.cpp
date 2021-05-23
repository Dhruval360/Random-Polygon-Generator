#include<iostream>
#include<stack>
#include<math.h>
#include<set>
#include"Polygon.hpp"

//CONVEX HULL ALGORITHM IS WORKING LIKE A CHARM
//CHECK POLYGON GENERATION ALGORITHM

using namespace std;

#define inf INFINITY
#define col 0
#define cw  1
#define ccw 2
//#define DEBUG  // Uncomment to enable debug

extern double timer;
extern float Scale;

typedef struct edge{
	pair<double,double> startVertex;
	pair<double,double> endVertex;
}Edge;

/******************************* Helper Functions *******************************/
#ifdef DEBUG
	void pointPrinter(pair<double,double> &pt){
		cout << '(' << "X:" << pt.first << ','<< "y:" <<pt.second << ')' << ',' << '\n';
	}

	void edgePrinter(Edge &e){
		cout << "From ";
		pointPrinter(e.startVertex);
		cout << "To ";
		pointPrinter(e.endVertex);
	}
#endif

bool vertexComparator(pair<double,double> &a, pair<double,double> &b){
	return (a.first == b.first && a.second == b.second);
}

bool vertexNegComparator(pair<double,double> &a, pair<double,double> &b){
	return (a.first != b.first || a.second != b.second);
}

double point2lineDist(pair<double,double> &p, pair<double,double> &p1, pair<double,double> &p2){
	//line segment starting point to the point vector
	double A = p.first-p1.first;
	double B = p.second - p1.second;
	//line segment vector
	double C = p2.first - p1.first;
	double D = p2.second - p1.second;

	//dot product of point vec and line segment
	double dot = (A*C)+(B*D);
	//length of line segment
	double len_seg = (C*C)+(D*D);

	//cases
	double param = -1;
	if(len_seg !=0) //non zero len line segment
	{
		param = (dot/len_seg);
	}
	
	double xx,yy;
	if(param<0){
		xx = p1.first;
		yy = p1.second;
	}else if(param >1){
		xx = p2.first;
		yy = p2.second;
	}else{
		xx = p1.first + (param*C);
		yy = p1.second + (param*D);
	}

	double dx = p.first-xx;
	double dy = p.second-yy;
	return sqrt((dx*dx)+(dy*dy));
}

/*
	Function to find orientation
	Consider points A, B, C. If slope of A->C is GREATER than slope of A->B, then A, B, C are in ccw dir
*/
bool isccw(pair<double,double> &a,pair<double,double> &b, pair<double,double> &c){
	return (c.second-a.second)*(b.first-a.first) > (b.second-a.second)*(c.first-a.first);
}

/*
	Consder the 4 points A, B, C, D. If segment CD intersects segment AB, then ACD and BCD
	will have opposite orientation i.e one will be having clockwise orientation and the other will
	be having anticlockwise orientation. Similarly triangle ABD and ABC will have opposite orientation
*/
bool isIntersectingUtil(pair<double,double> &a, pair<double,double> &b, pair<double,double> &c, pair<double,double> &d){
	return (isccw(a,c,d) != isccw(b,c,d) && isccw(a,b,d) != isccw(a,b,c));
}

// Function to check if the edge e is intersecting with any of the edges of the polygon
bool isIntersectingEdge(vector <Edge> &edges, Edge e){
	// Check for all the edges of the polygon with the edge e for intersection
	for(auto iterE : edges){
		// If edge to be checked and iterEdge share same end points, then they can lie on each
		// At max but cannot intersect, hence check for that case
		if(vertexNegComparator(e.startVertex,iterE.startVertex) && 
		vertexNegComparator(e.endVertex,iterE.startVertex)){
			if(vertexNegComparator( e.startVertex ,iterE.endVertex) &&
				vertexNegComparator( e.endVertex,iterE.endVertex)){
				// Check for orientation of edge points
				if(isIntersectingUtil(iterE.startVertex,iterE.endVertex,e.startVertex,e.endVertex))
					return true;
			}
		}
	}
	return false;
}

// Function to check if the 2 edges we are creating will be valid or not the edges are valid if they do not intersect the polygon
bool isValidEdge(vector <Edge> &edges, Edge &e, pair<double, double> &p){
	//create an edge from e.start to the point
	Edge e1;
	e1.startVertex = e.startVertex;
	e1.endVertex = p;
	//create an edge from p to e.end
	Edge e2;
	e2.startVertex = p;
	e2.endVertex = e.endVertex;
	//check if the edges are intersecting with the polygon
	if(!isIntersectingEdge(edges,e1) && !isIntersectingEdge(edges,e2)) return true;
	return false;
}

bool edgeComparator(Edge e1,Edge e2){
	return (vertexComparator(e1.startVertex,e2.startVertex) &&
	 vertexComparator(e1.endVertex,e2.endVertex));
}

//function to find index of entity in the vector
int indexInEdgesVec(vector <Edge> arr,Edge k){
	for(int i = 0;i<arr.size();i++){
		if(edgeComparator(arr.at(i),k)){
			return i;
		}
	}
	//printf("\nElement not found\n");
    return -1;
}

Edge myFind(vector <Edge> &edges, int &i, bool* status){
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
int orientationOfPoints(pair<double,double> p, pair<double,double> &q, pair<double,double> &r){
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
double euclidsDist(pair<double,double> &p1, pair<double,double> &p2){
	return ((p2.first-p1.first)*(p2.first-p1.first))+((p2.second-p1.second)*(p2.second-p1.second));
}

// Sorting based on polar angles from the anchor point
int polAngSorter(const void* vp1, const void* vp2, void* anchor){
	pair<double,double> p1 = *((pair<double,double>*)vp1);
	pair<double,double> p2 = *((pair<double,double>*)vp2);
	pair<double,double> p  = *((pair<double,double>*)anchor);
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

void generateconvexHull(vector<pair<double,double>> coordinates,
vector <pair<double,double>> &resHull){
	//finding the bottom most point out of all the points
	pair<double,double> min_point = coordinates.at(0);
	int min_index = 0;
	int i = 0;
	for(auto pt : coordinates){
		//check which is more bottom
		if(pt.second<min_point.second){
			min_point = pt;
			min_index = i;
		}
		//if y coord is same the check for x coordinate
		else if(pt.second == min_point.second && pt.first < min_point.first){
			min_point = pt;
			min_index = i;
		}
		i++;
	}
	//cout<<"Bottom most point : \n";
	//pointPrinter(min_point);

	//swap the pos of bottom most point with the first index
	iter_swap(coordinates.begin()+0,coordinates.begin()+min_index);
	//cout <<"After swapping : \n";
	for(int i = 0;i<coordinates.size();i++){
		//pointPrinter(points[i]);
	}

	//sort the remaining n-1 points with respect to this anchor point
	//ie the first point based on the POLAR angle in anti clock wise 
	//dir because its a convex hull
	auto p = coordinates[0];
	//SORT does NOT work,fix that - maybe we need to send boolean for SORT
	//sort(points.begin()+1,points.end(),polAngSorter);
	//qsort(&points[1],points.size()-1,sizeof(pair<double,double>),polAngSorter);
	qsort_r(&coordinates[1],coordinates.size()-1, sizeof(pair<double,double>), polAngSorter, &p);
	////cout << "Smallest polar angle point from ";pointPrinter(p);
	//cout << "After sorting\n";
	for(int i = 0;i<coordinates.size();i++){
		//pointPrinter(points[i]);
	}
	//pointPrinter(points[1]);
	//now remove all the points with same polar angle from anchor point
	//except for the farthest point
	//the array is already sorted based on polar angles and also when 
	//the polar angles where same we consdered their dist from p0 as the 
	//factor for sorting.Hence the farthest point with similar polar
	//angles is located at the end
	for(int i = 1;i<coordinates.size();i++){
		//iter through the sub vector to find same polar angle points
		int j = i;
		//count the indexes with same polar angles
		while(j< (coordinates.size()-1) && 
			orientationOfPoints(p,coordinates[j],coordinates[j+1]) == col){
			j++;
		}
		//erase from i to j if i!=j
		if(i!=j){
			//cout << "i "<<i<<endl;
			//cout << "j "<<j<<endl;
			//cout << j-i << " Dups found,removing them..\n";
			coordinates.erase(coordinates.begin()+i,coordinates.begin()+(j));
		}
	}

	//cout << "After removing duplicates : \n";
	//for(int i = 0;i<points.size();i++){
		//pointPrinter(points[i]);
	//}

	//if the new vec has LT 3 points,then no convex hull formed
	if(coordinates.size()<3) return ;

	//create the stack to store the points
	//push the first three points p0,p1,p2
	stack <pair<double,double>> stk;
	//cout << "Pushing.....\n";
	stk.push(coordinates[0]);
	//pointPrinter(points[0]);
	stk.push(coordinates[1]);
	//pointPrinter(points[1]);
	stk.push(coordinates[2]);
	//pointPrinter(points[2]);
	/*
	|p2|
	|p1|
	|p0|
	----
	*/

	//now start analysing the points from p3
	for(int i = 3;i<coordinates.size();i++){
		//cout << "Inside For loop \n";
		//check the orientation ie angle formed from the point at the top 
		//of the stack with the curr iter point,if its to thee right ie 
		//cw(CONCAVE),then violates convex hull rule
		/*while(stk.size()>1){
			pair<double,double> theTop = stk.top();
			pair<double,double> secondTop = justBelowTop(&stk);
			//check the ori of p,p1,p2 ie b/w segments p,p1 and p1,p2
			if(orientationOfPoints(secondTop,theTop,points[i]) != ccw){
				//if not ccw,then concave,hence rem point
				stk.pop();
			}
		}*/
		//pointPrinter(points[i]);
		while(stk.size()>1 && 
			orientationOfPoints(justBelowTop(&stk),stk.top(),coordinates[i]) != ccw)
				stk.pop();
		//push the next point under consideration
		stk.push(coordinates[i]);
	}
	//cout<<"stk size:"<<stk.size()<<endl;
	//cout<<"points arr size "<<points.size()<<endl;

	//stack has all the points of the HULL
	//cout<<"Result : \n";
	while(!stk.empty()){
		pair<double,double> temp = stk.top();
		//pointPrinter(temp);
		resHull.push_back(temp);
		stk.pop();
	}
	//cout <<"\n************ResHULL************\n";
	//cout <<"{";
	//for(auto p : resHull){
		//cout <<"{"<<p.first<<","<<p.second<<"},";
	//}
	//cout <<"}\n";
}

void generatePolygon(Polygon *polygon){
	//cout << "The size of polygon coordinates is " << polygon->numVertices<<endl;
	//cout << "{";
	//for(auto p : polygon->coordinates){
		//cout <<"{";
		//cout << p.first<<","<<p.second<<"},";
	//}
	//cout <<"}";
	vector <pair<double,double>> resHull; // The resultant CONVEX HULL
	//resHull.reserve(polygon->numVertices);
	vector <pair<double,double>> allPoints = polygon->coordinates;
	generateconvexHull(polygon->coordinates, resHull);

	vector <Edge> edges; // Edges of the convex hull
	for (int i = 0; i < (int)(resHull.size()-1); ++i){ // Creating edges of poly from convex hull vertices
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
		for(auto cv : resHull) if(cv.first == v.first && cv.second == v.second){
			 isPresent = true;
			 break;
		}
		if(!isPresent) interiorPoints.push_back(v);
	}

	#ifdef DEBUG
		cout << "Interior Points : ";
		for(auto pt : interiorPoints) pointPrinter(pt);
		cout << "\nThe edges of convex Hull are : \n";
		for(auto e : edges) edgePrinter(e);
	#endif

	//while there are interior points remaining
	while(interiorPoints.size()> 0){
		//initially the distance iallpoins infinite
		double minDist = inf;
		//edge to be removed
		Edge toRemEdge;
		//the nearest point
		pair<double,double> nearestPoint;
		//iterate over all edges
		for(auto e : edges){
			//iterate over all interior points for each edge
			for(auto ip : interiorPoints){
				//find distance between line(edge under consideration) and 
				//point(interior point under consideration)
				double currDist = point2lineDist(ip,e.startVertex,e.endVertex);
				//cout << "Curr Dist " << currDist<<endl;
				//cout<<"Dist of edge ";edgePrinter(e);//cout<<" To point ";
				//pointPrinter(ip);//cout<<" is ";//cout<<currDist;//cout<<endl;
				if(currDist < minDist)
					if(isValidEdge(edges,e,ip)){
						//this can be the minDist between point and edge
						minDist = currDist;
						toRemEdge = e;
						nearestPoint = ip;
				}
			}
		}
		//cout <<"Choosen : \n";
		//cout<<"Dist of edge ";edgePrinter(toRemEdge);//cout<<" To point ";
		//pointPrinter(nearestPoint);//cout<<" is ";//cout<<minDist;//cout<<endl;
		//cout<<"End\n";
		//by here we would have the closest point to edge data
		//the index of the toRemEdge in edges array
		int i = indexInEdgesVec(edges,toRemEdge);
		//hence replace the "toRemEdge" with an edge from toRemEdge.start to nearestPoint
		Edge e;
		e.startVertex = toRemEdge.startVertex;
		e.endVertex = nearestPoint;
		edges.at(i) = e;
		//insert the edge from nearestPoint to toRemEdge.end
		Edge f;
		f.startVertex = nearestPoint;
		f.endVertex = toRemEdge.endVertex;
		//find iterator of the edge at i+1 and insert the new edge
		/*
			//auto j = find(edges.begin(),edges.end(),edges.at(i+1));
			//bool isFindSuccess = false;
			//auto j = myFind(edges,i+1,&isFindSuccess);
		*/
		auto j = edges.begin()+(i+1);
		edges.insert(j,f);
		//if(isVerbose) cout << "#";
		//add the point to the current vertices of the polygon ie to the resHull
		//resHull.insert(resHull.begin()+ i,nearestPoint);
		//remove the point from the interior points array
		for(auto i = interiorPoints.begin();i<interiorPoints.end();i++){
			if(vertexComparator(*i,nearestPoint)){
				interiorPoints.erase(i);
				break;
			}
		}
	}
	//clear the polygon coordinates of the previous elements
	vector <pair<double,double>> polyPoints;
	for(auto e : edges){
		polyPoints.push_back(e.startVertex);
		#ifdef DEBUG
			edgePrinter(e);
		#endif
	}
	polygon->coordinates = polyPoints;
}

/********************************* The Algorithm *********************************/
void naivePolygon(Polygon* polygon, bool verbose){
	start_timer(start);

	generatePolygon(polygon);    // Generate the polygon with ordered points
	end_timer(start, timer);
	if(verbose) printf("Number of vertices = %3u | Time taken for generation = %lf s\n",  polygon->numVertices, timer);
}