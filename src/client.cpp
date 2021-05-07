#include<iostream>
#include"polygon.hpp"
using namespace std;
int main(){
    cout << "Welcome to the Polygon Generator Project" << endl;
    int num_vertices;
    char filename[100];
    double min, max;
    cout << "Enter number of vertices in the polygon : ";
	cin >> num_vertices; 
	cout << endl;
	cout << "Enter lower bound : ";
	cin >> min; 
	cout << endl;
	cout << "Enter upper bound : ";
	cin >> max; 
	cout << endl;
	cout << "Enter the wkt file name : ";
	cin >> filename; 
	cout << endl;
	srand(time(NULL));
	point *points_array = new point[num_vertices];
	random_points(points_array, num_vertices, min, max);
	space_partition(points_array, num_vertices);
	write_to_wkt(points_array, num_vertices, filename);
	delete[] points_array;
    /*unsigned num_polygons;
    cout << "Enter number of Polygons to be generated : ";
	cin >> num_polygons; 
	cout << endl;
    unsigned numVerts = 10;
    Polygon *p = new Polygon[num_polygons];
    for(unsigned i = 0; i < num_polygons; i++){
        p[i] = Polygon(numVerts + i);
        polygonGenerator(0, 0, 50, 0.0, 0.0, p[i]);
    } 
    write(p, num_polygons);*/
    return 0;
}