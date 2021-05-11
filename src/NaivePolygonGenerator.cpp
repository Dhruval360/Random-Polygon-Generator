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

using namespace std;
extern double timer;

#define col 0
#define cw  1
#define ccw 2

typedef struct pair{
    double x; //x and y coor
    double y;
}doublePoint;

typedef struct edge{
	doublePoint startVertex;
	doublePoint endVertex;
}Edge;

typedef struct polygon{
	vector <doublePoint>* vertices;
	vector <Edge>* edges;
}myPolygon;

/************************************ Globals ************************************/
doublePoint p; // Anchor point to polar sort the rem points
static vector <doublePoint> resHull; // The resultant CONVEX HULL
static vector <doublePoint> allPoints; // All points needed to be included in POLYGON - Supplied as random points
static vector <doublePoint> polyPoints; // The result points

double inf = INFINITY;

/******************************* Helper Functions *******************************/
// Comparator to insert into set
bool cmpPoints(const doublePoint &a,const doublePoint &b){
	return ((a.x < b.x) || (a.y <b.y));
}

void randDoubleGen(int n){
	random_device rd; // Random device that initiates a random engine
	mt19937 random_engine(rd()); // The random engine
	uniform_real_distribution<double> unif(-1000,1000); // The distribution used
	set <doublePoint,bool(*)(const doublePoint&,const doublePoint&)> temp(&cmpPoints); // A set to store non duplicate points only
	for(int i = 0; (int)temp.size() < n; i++){
		double x = unif(random_engine);
		double y = unif(random_engine);
		doublePoint p;
		p.x = x;
		p.y = y;
		temp.insert(p);
	}
	for(auto pt : temp)	allPoints.push_back(pt);
}

void pointPrinter(doublePoint pt){
    //cout << '(' << "X:" << pt.x << ','<< "y:" <<pt.y << ')' << ',' << endl;
}

bool vertexComparator(doublePoint a,doublePoint b){
	return (a.x == b.x && a.y == b.y);
}

bool vertexNegComparator(doublePoint a,doublePoint b){
	return (a.x != b.x || a.y != b.y);
}


double point2lineDist(doublePoint p,doublePoint p1,doublePoint p2){
	// Line segment starting point to the point vector
	double A = p.x-p1.x;
	double B = p.y - p1.y;
	
	// Line segment vector
	double C = p2.x - p1.x;
	double D = p2.y - p1.y;

	double dot = (A*C)+(B*D); // Dot product of point vec and line segment
	double len_seg = (C*C)+(D*D); // Length of line segment

	// Cases
	double param = -1;
	if(len_seg !=0) param = ((double)dot/(double)len_seg); // Non zero len line segment
	
	double xx,yy;
	if(param<0){
		xx = p1.x;
		yy = p1.y;
	}
	else if(param >1){
		xx = p2.x;
		yy = p2.y;
	}
	else{
		xx = p1.x + (param*C);
		yy = p1.y + (param*D);
	}

	double dx = p.x-xx;
	double dy = p.y-yy;
	return sqrt((dx*dx)+(dy*dy));
}

// Function to find orientation
// Consider points a,b,c.If slope of A->C is GREATER than slope of A->B,then A,B,c are in ccw dir
bool isccw(doublePoint a,doublePoint b,doublePoint c){
	return (c.y-a.y)*(b.x-a.x) > (b.y-a.y)*(c.x-a.x);
}

//consder the 4 points A,B,C,D.If segment CD intersects segment AB,then ACD and BCD
//will have opposite orientation ie one will be having clockwise orientation and the other will
//be having anticlockwise orientation.
//similarly triangle ABD and ABC will have opposite orientation
bool isIntersectingUtil(doublePoint a,doublePoint b,doublePoint c,doublePoint d){
	//float m1 = (float)(b.y-a.y)/(float)(b.x-a.x);
	//float m2 = (float)(d.y-c.y)/(float)(d.x-c.x);
	////printf("\nThe slopes are %lf and %lf\n",round(m1),round(m2));
	//if(round(m1)==round(m2)) return true;
	return (isccw(a,c,d) != isccw(b,c,d) && isccw(a,b,d) != isccw(a,b,c));
}

//function to check if the edge e is intersecting with any of the edges of the polygon
bool isIntersectingEdge(vector <Edge> edges,Edge e){
	//check fr all the edges of the polygon with the edge e for intersection
	for(auto iterE : edges){
		//if edge to be checked and iterEdge share same end points,then they can lie on each
		//at max but cannot intersect,hence check for that case
		if(vertexNegComparator(e.startVertex,iterE.startVertex) && 
		vertexNegComparator(e.endVertex,iterE.startVertex)){
			if(vertexNegComparator( e.startVertex ,iterE.endVertex) &&
				vertexNegComparator( e.endVertex,iterE.endVertex)){
				//check for orientation of edge points
				if(isIntersectingUtil(iterE.startVertex,iterE.endVertex,
					e.startVertex,e.endVertex))
					return true;
			}
		}
	}
	return false;
}

//function to check if the 2 edges we are creating will be valid or not
//the edges are valid if they do not intersect the polygon
bool isValidEdge(vector <Edge> edges, Edge e,doublePoint p){
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
	for(int i = 0;i<(int)arr.size();i++){
		if(edgeComparator(arr.at(i),k)){
			return i;
		}
	}
	//printf("\nElement not found\n");
    return -1;
}

int indexInPointsVec(vector <doublePoint> arr,doublePoint k){
	for(int i = 0;i<(int)arr.size();i++){
		if(vertexComparator(arr.at(i),k)){
			return i;
		}
	}
	//printf("\nElement not found\n");
    return -1;
}

Edge myFind(vector <Edge> edges,int i,bool* status){
	for(int i = 0;i<(int)edges.size();i++){
		if(edgeComparator(edges[i],edges[i])){
			*status = true;
			return edges[i];
		}
	}
	//printf("\nNo edge found!\n");
	*status = false;
	return edges[0];
}

//void pointPrinter(doublePoint pt){
    ////cout << '(' << "x:" << pt.x << ','<< "y:" <<pt.y << ')' << ',' << " ";
//}

void edgePrinter(Edge e){
	//cout << "From ";
	pointPrinter(e.startVertex);
	//cout << "To ";
	pointPrinter(e.endVertex);
}

//orientation of 3 pints depends on their slope
//if diff of slopes is po,thenthere is RIGHT turn ie cw else if neg then
//ccw ie LEFT or else collinear if 0
int orientationOfPoints(doublePoint p,doublePoint q,doublePoint r){
	//find diff between slope
	double resM = (((q.y-p.y)*(r.x-q.x)) - ((q.x-p.x)*(r.y-q.y)));
	//cout << "Orientation:"<<resM<<endl;
	if(resM == 0) return col; //collinear
	if(resM>0) return cw;
	return ccw;
}

//return (euclidsDistance)^2 of the points p1 and p2
double euclidsDist(doublePoint p1,doublePoint p2){
	return ((p2.x-p1.x)*(p2.x-p1.x))+((p2.y-p1.y)*(p2.y-p1.y));
}

void oriPrinter(int x){
	if(x==col){
		//cout<<"Collinear\n";
		return;
	}
	if(x==cw) {
		//cout<<"Clockwise\n";
		return;
	}
	//cout<<"Anti clockwise\n";
}

//sorting based on polar angles from the anchor point
//int polAngSorter(doublePoint p1,doublePoint p2){
int polAngSorter(const void* vp1,const void* vp2){
	doublePoint p1 = *((doublePoint*)vp1);
	doublePoint p2 = *((doublePoint*)vp2);
	int ori = orientationOfPoints(p,p1,p2);
	switch(ori){
		//if they are collinear then use euclids distance
		case col:
		if(euclidsDist(p,p2)>=euclidsDist(p,p1)) return -1;
		return 1;

		case cw:
		return 1;

		case ccw:
		return -1;

		default:
		//printf("\nUnable to sort?\n");
		return 0;
		break;
	}
}

//get the element just below the top
doublePoint justBelowTop(stack <doublePoint>* stk){
	doublePoint theTop = stk->top();
	stk->pop();
	doublePoint retVal = stk->top();
	stk->push(theTop);
	return retVal;
}

//function to write to text file
bool writeWKT(vector <doublePoint> allPoints){
	string txt = "";
	for(int i = 0;i< (int)allPoints.size();i++){
		txt += to_string(allPoints[i].x);
		txt += " ";
		txt += to_string(allPoints[i].y);
		if(i != (int)allPoints.size()-1){
			txt += ", ";
		}
	}
	string temp = string("POLYGON ") + string("(") + txt + string(")") + "\n";
	ofstream outData;
	outData.open("data.wkt",ios::app);
	if(!outData) {
		//cout << "Unable to open\n";
		return false;
	}
	outData << temp;
	outData.close();
	return true;
}

void convexHull(vector <doublePoint> points){
	//finding the bottom most point out of all the points
	doublePoint min_point = points.at(0);
	int min_index = 0;
	int i = 0;
	for(auto pt : points){
		//check which is more bottom
		if(pt.y<min_point.y){
			min_point = pt;
			min_index = i;
		}
		//if y coord is same the check for x coordinate
		else if(pt.y == min_point.y && pt.x < min_point.x){
			min_point = pt;
			min_index = i;
		}
		i++;
	}
	//cout<<"Bottom most point : \n";
	pointPrinter(min_point);

	//swap the pos of bottom most point with the first index
	iter_swap(points.begin()+0,points.begin()+min_index);
	//cout <<"After swapping : \n";
	for(int i = 0;i<(int)points.size();i++){
		pointPrinter(points[i]);
	}

	//sort the remaining n-1 points with respect to this anchor point
	//ie the first point based on the POLAR angle in anti clock wise 
	//dir because its a convex hull
	p = points[0];
	//SORT does NOT work,fix that - maybe we need to send boolean for SORT
	//sort(points.begin()+1,points.end(),polAngSorter);
	qsort(&points[1],points.size()-1,sizeof(doublePoint),polAngSorter);
	////cout << "Smallest polar angle point from ";pointPrinter(p);
	//cout << "After sorting\n";
	for(int i = 0;i<(int)points.size();i++){
		pointPrinter(points[i]);
	}
	//pointPrinter(points[1]);
	//now remove all the points with same polar angle from anchor point
	//except for the farthest point
	//the array is already sorted based on polar angles and also when 
	//the polar angles where same we consdered their dist from p0 as the 
	//factor for sorting.Hence the farthest point with similar polar
	//angles is located at the end
	for(int i = 1;i<(int)points.size();i++){
		//iter through the sub vector to find same polar angle points
		int j = i;
		//count the indexes with same polar angles
		while(j< (int)(points.size()-1) && 
			orientationOfPoints(p,points[j],points[j+1]) == col){
			j++;
		}
		//erase from i to j if i!=j
		if(i!=j){
			//cout << "i "<<i<<endl;
			//cout << "j "<<j<<endl;
			//cout << j-i << " Dups found,removing them..\n";
			points.erase(points.begin()+i,points.begin()+(j));
		}
	}

	//cout << "After removing duplicates : \n";
	for(int i = 0;i<(int)points.size();i++){
		pointPrinter(points[i]);
	}

	//if the new vec has LT 3 points,then no convex hull formed
	if(points.size()<3) return ;

	//create the stack to store the points
	//push the first three points p0,p1,p2
	stack <doublePoint> stk;
	//cout << "Pushing.....\n";
	stk.push(points[0]);
	pointPrinter(points[0]);
	stk.push(points[1]);
	pointPrinter(points[1]);
	stk.push(points[2]);
	pointPrinter(points[2]);
	/*
	|p2|
	|p1|
	|p0|
	----
	*/

	//now start analysing the points from p3
	for(int i = 3;i<(int)points.size();i++){
		//cout << "Inside For loop \n";
		//check the orientation ie angle formed from the point at the top 
		//of the stack with the curr iter point,if its to thee right ie 
		//cw(CONCAVE),then violates convex hull rule
		/*while(stk.size()>1){
			doublePoint theTop = stk.top();
			doublePoint secondTop = justBelowTop(&stk);
			//check the ori of p,p1,p2 ie b/w segments p,p1 and p1,p2
			if(orientationOfPoints(secondTop,theTop,points[i]) != ccw){
				//if not ccw,then concave,hence rem point
				stk.pop();
			}
		}*/
		pointPrinter(points[i]);
		while(stk.size()>1 && 
			orientationOfPoints(justBelowTop(&stk),stk.top(),points[i]) != ccw)
				stk.pop();
		//push the next point under consideration
		stk.push(points[i]);
	}
	//cout<<"stk size:"<<stk.size()<<endl;
	//cout<<"points arr size "<<(int)points.size()<<endl;

	//stack has all the points of the HULL
	//cout<<"Result : \n";
	while(!stk.empty()){
		doublePoint temp = stk.top();
		pointPrinter(temp);
		resHull.push_back(temp);
		stk.pop();
	}
}

void generateConvexHull(){
	/*vector <doublePoint> points = {{0, 3}, {1, 1}, {2, 2}, {4, 4},
                      {0, 0}, {1, 2}, {3, 1}, {3, 3}};*/
    convexHull(allPoints);
}

void generatePolygon(){
	//points in the convex Hull
	//vector <doublePoint> resHull = {{-5,-3},{-1,1},{0,0},{1,-4},{-1,-5}};
    //vector <doublePoint> resHull = {{0,0},{0,3},{3,1},{4,4}};              
	//resHull = util(allPoints);
	//in real,get from utils function

	//vertices of the convex hull
	myPolygon* poly = (myPolygon*)malloc(sizeof(myPolygon));
	poly->vertices = &resHull;//convex hull vertices only
	vector <Edge> edges;
	poly->edges = &edges; //edges of the convex hull only

	//creating edges of poly from convex hull vertices
	for (int i = 0; i < (int)(resHull.size()-1); ++i){
		Edge e1;
		e1.startVertex = resHull[i];
		e1.endVertex = resHull[i+1];
		edges.push_back(e1);
	}
	//connect the starting and the last vertex
	Edge e;
	e.startVertex = resHull[resHull.size()-1];
	e.endVertex = resHull[0];
	edges.push_back(e);

	//interior points are those points that are not part of the convex hull vertices
	//interior points are correctly generated
	vector <doublePoint> interiorPoints;
	for(auto v : allPoints){
		bool isPresent = false;
		for(auto cv : resHull){
			if(cv.x == v.x && cv.y == v.y) isPresent = true;
		}
		if(!isPresent){
			interiorPoints.push_back(v);
		}
	}

	/*//cout << "Interior Points : ";
	for(auto pt : interiorPoints){
		pointPrinter(pt);
	}
	//cout << endl;*/

	//cout << "The edges of convex Hull are : \n";
	for(auto e : edges){
		edgePrinter(e);
		//cout << endl;
	}

	//while there are interior points remaining
	while(interiorPoints.size()> 0){
		//initially the distance is infinite
		double minDist = inf;
		//edge to be removed
		Edge toRemEdge;
		//the nearest point
		doublePoint nearestPoint;
		//iterate over all edges
		for(auto e : edges){
			//iterate over all interior points for each edge
			for(auto ip : interiorPoints){
				//find distance between line(edge under consideration) and 
				//point(interior point under consideration)
				double currDist = point2lineDist(ip,e.startVertex,e.endVertex);
				//cout << "Curr Dist " << currDist<<endl;
				//cout<<"Dist of edge ";edgePrinter(e);//cout<<" To point ";
				pointPrinter(ip);//cout<<" is ";//cout<<currDist;//cout<<endl;
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
		pointPrinter(nearestPoint);//cout<<" is ";//cout<<minDist;//cout<<endl;
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
		/*
		Edge z;
		doublePoint p;
		z.startVertex = p;
		z.endVertex = p; 
		edges.resize(edges.size() + 1, z);
		*/
		edges.insert(j,f);
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
	for(auto e : edges){
		polyPoints.push_back(e.startVertex);
		edgePrinter(e);
	}
	free(poly);
}

/********************************* The Algorithm *********************************/
void naivePolygon(Polygon* polygon,bool verbose){
	start_timer(start);
	randDoubleGen(polygon->numVertices);
	generateConvexHull(); // Generate the convex hull
	generatePolygon();    // Generate the polygon with ordered points
	
	// Push in the polygon ordered slides
	for(unsigned i = 0;i<polygon->numVertices;i++){
		polygon->coordinates.push_back({polyPoints[i].x,
		polyPoints[i].y});
	}
	allPoints.clear();
	polyPoints.clear();
	resHull.clear();
	end_timer(start, timer);
	if(verbose) printf("Number of vertices = %3u | Time taken for generation = %lf s\n",  polygon->numVertices, timer);
}