#include<iostream>
#include<fstream>
#include<random>
#include<cstring>
#include<utility>
#include "Polygon.hpp"
#define DEBUG 0
using namespace std;

/*double random_num();
void random_points(Polygon *ps, int count, double min, double max);
pair<double,double> random_point_segment(const pair<double,double> &s, const pair<double,double> &e);
bool to_left(const pair<double,double> &s, const pair<double,double> &e, const pair<double,double> &p);
void swap(Polygon *ps, int i, int j);
void recursive_partioning(Polygon *ps, int count, int l, int r);
void space_partition(Polygon *ps, int count, int min, int max);
//writes the randomly generated points to the wkt file
void write_to_wkt(Polygon *polygon, int num_vertices, char *filename);*/
/*
int main(){
	cout << "Welcome to the Polygon Generator Project" << endl;
    unsigned num_vertices, num_polygons;
    char filename[100];
    double min, max;
	/*cout << "Enter the wkt file name : ";
	cin >> filename; 
	cout << endl;
	cout << "Enter number of polygons: ";
	cin >> num_polygons; 
	cout << endl;
	srand(time(NULL));

	Polygon *polygon = new Polygon[num_polygons];
	for(int i = 0; i < num_polygons; i++){
		cout << "Enter number of vertices in the polygon : ";
		cin >> num_vertices; 
		cout << endl;
		cout << "Enter lower bound : ";
		cin >> min; 
		cout << endl;
		cout << "Enter upper bound : ";
		cin >> max; 
		cout << endl;	
		polygon[i] = Polygon(num_vertices);
		polygon[i].Generator2(num_vertices, min, max);
		//random_points(polygon, count, min, max);
		//space_partition(polygon, count);
		//write_to_wkt(polygon, num_vertices, filename);
	}	
	write(polygon, num_polygons);
	return 0;
}*/

//implementation of the space partition algorithm to generate random polygons
void space_partition(Polygon *polygon, int num_vertices, int min, int max){
	//call this function to initialise the polygon with a random set of points
	random_points(polygon, num_vertices, min, max);
	//choose the start point as the first element and any random point as the initial final element
	int end_index = rand()%(num_vertices - 1) + 1;
	//note the start and end points
	pair<double, double> start_point = polygon->coordinates[0], end_point = polygon->coordinates[end_index];
	//exchange the 2nd element with the randomly chosen last element
	swap(polygon, 1, end_index);
	//
	int temp_lr = 2, temp_rl = num_vertices - 1;
	while(temp_lr <= temp_rl){
		if(DEBUG){
			cout << "temp_lr = " << temp_lr << " temp_rl = " << temp_rl << endl;
		}
		while(temp_lr < num_vertices && to_left(start_point, end_point, polygon->coordinates[temp_lr])){
			temp_lr++;
		}
		while(temp_rl > 1 && !to_left(start_point, end_point, polygon->coordinates[temp_rl])){
			temp_rl--;
		}
		if(temp_lr <= temp_rl){
			swap(polygon, temp_lr, temp_rl);
			temp_lr++;
			temp_rl--;
		}
	}
	swap(polygon, 1, temp_rl);
	//call the recursive functions to solve for the left and right part of the problem
	recursive_partioning(polygon, num_vertices, 0, temp_rl);
	recursive_partioning(polygon, num_vertices, temp_rl, num_vertices);
}

//recursive function to solve the problem by the means of divide and conquer
void recursive_partioning(Polygon *polygon, int num_vertices, int begin, int end){
	if(end > begin + 1){
		pair<double,double> start_point = polygon->coordinates[begin];
		pair<double,double> end_point = (end == num_vertices) ? polygon->coordinates[0] : polygon->coordinates[end];
		pair<double,double> random_start_point = random_point_segment(start_point, end_point);
		int random_index = (rand()%(end - begin - 1)) + begin + 1;
		pair<double,double> random_end_point = polygon->coordinates[random_index];
		swap(polygon, begin + 1, random_index);
		int temp_lr = begin + 2, temp_rl = end - 1;
		bool flag = to_left(random_start_point, random_end_point, polygon->coordinates[begin]);
		while(temp_lr <= temp_rl){
			while(temp_lr < end && to_left(random_start_point, random_end_point, polygon->coordinates[temp_lr]) == flag){
				temp_lr++;
			}
			while(temp_rl > begin + 1 && to_left(random_start_point, random_end_point, polygon->coordinates[temp_rl]) != flag){
				temp_rl--;
			}
			if(temp_lr <= temp_rl){
				swap(polygon, temp_lr, temp_rl);
				temp_lr++;
				temp_rl--;
			}
		}
		swap(polygon, begin + 1, temp_rl);
		//call the recursive functions to solve for the left and right part of the problem
		recursive_partioning(polygon, num_vertices, begin, temp_rl);
		recursive_partioning(polygon, num_vertices, temp_rl, end);
	}
}

//exchanges contents of the said indices in the vector
void swap(Polygon *polygon, int first_index, int second_index){
	if(DEBUG){
		cout << "Before swapping " << polygon->coordinates[first_index].first << " " << polygon->coordinates[first_index].second 
		<< " " << polygon->coordinates[second_index].first << " " << polygon->coordinates[second_index].second << endl;
	}
	//using in-built swap function to swap the tuples
	polygon->coordinates[second_index].swap(polygon->coordinates[first_index]);
	if(DEBUG){
		cout << "After swapping " << polygon->coordinates[first_index].first << " " << polygon->coordinates[first_index].second 
		<< " " << polygon->coordinates[second_index].first << " " << polygon->coordinates[second_index].second << endl;
	}
}
//this function returns a value based on which side the test point is w.r.t the line
bool to_left(const pair<double,double> &start_point, const pair<double,double> &end_point, const pair<double,double> &test_point){
	if(DEBUG){
		cout << "Start point " << start_point.first << " " << start_point.second << endl;
		cout << "End point " << end_point.first << " " << end_point.second << endl;
		cout << "Test point " << test_point.first << " " << test_point.second << endl;
		cout << "Distance " << (end_point.first - start_point.first) * (test_point.second - start_point.second) - (end_point.second - start_point.second) * (test_point.first - start_point.first) << endl; 
	}
	return (end_point.first - start_point.first) * (test_point.second - start_point.second) <= 
	(end_point.second - start_point.second) * (test_point.first - start_point.first);
}

//returns a random point on the line segment
pair<double,double> random_point_segment(const pair<double,double> &start_point, const pair<double,double> &end_point){
	double random_ratio = random_num();
	pair<double,double> random_point;
	random_point.first = start_point.first + (end_point.first - start_point.first) * random_ratio;
	random_point.second = start_point.second + (end_point.second - start_point.second) * random_ratio;  
	return random_point;
}

//generates a random set of points from which a random polygon is constructed
void random_points(Polygon *polygon, int num_vertices, double min, double max){
	for (int i = 0; i < num_vertices; i++){
		polygon->coordinates.push_back(pair<double,double>(min + (max - min) * random_num(), min + (max - min) * random_num()));
		if(DEBUG){
			cout << "Random point " << polygon->coordinates[i].first << " " << polygon->coordinates[i].second << endl;
		}
	}
}

//generates a random number between 0 and 1
double random_num(){
	return rand()/(double)RAND_MAX;
}
/*
//writes the generated results into a wkt file
void write_to_wkt(Polygon *polygon, int num_vertices, char *filename){
	ofstream file(filename, ofstream::out);
	for(int i = 0; i < num_vertices; i++){
		if(i == 0){
			file << "POLYGON ((";
		}
		else if(i == num_vertices-1){
			file << polygon->coordinates[i].first << " " << polygon->coordinates[i].second << ", ";
			file << polygon->coordinates[0].first << " " << polygon->coordinates[0].second << "))" << endl;
			if(DEBUG){
				cout << "Polygon successfully written to file" << endl;
			}
			return;
		}
		file << polygon->coordinates[i].first << " " << polygon->coordinates[i].second << ", ";
	}
}*/