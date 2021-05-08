#include<iostream>
#include<fstream>
#include<random>
#include<cstring>
#include<utility>
#include "Polygon.hpp"
#define DEBUG 0
using namespace std;

//generates random number between 0 and 1
static double random_num();
//initiatres the coordinate vector with num_vertices number of random points
static void random_points(Polygon *polygon, double min, double max);
//return a random point on the given line segment
static pair<double,double> random_point_segment(const pair<double,double> &start_point, const pair<double,double> &end_point);
//this function returns a value based on which side the test point is w.r.t the line
static bool to_left(const pair<double,double> &start_point, const pair<double,double> &end_point, const pair<double,double> &test_point);
//exchanges contents of the said indices in the vector
static void swap(Polygon *polygon, int first_index, int second_index);
//recursive function to solve the problem by the means of divide and conquer
static void recursive_partioning(Polygon *polygon, int begin, int end);
//helper function for partitioning
static void partition_logic(Polygon *polygon, int &temp_lr, int &temp_rl, int &begin, int &end, pair<double, double> start_point, pair<double, double> end_point, int flag);

//implementation of the space partition algorithm to generate random polygons
void space_partition(Polygon *polygon, int min, int max){
	//call this function to initialise the polygon with a random set of points
	random_points(polygon, min, max);
	//choose the start point as the first element and any random point as the initial final element
	int end_index = rand()%(polygon->numVertices - 1) + 1;
	//note the start and end points
	pair<double, double> start_point = polygon->coordinates[0], end_point = polygon->coordinates[end_index];
	//exchange the 2nd element with the randomly chosen last element
	swap(polygon, 1, end_index);
	int begin = 0, end = polygon->numVertices;
	//
	int temp_lr = 2, temp_rl = polygon->numVertices - 1;
	int flag = 1;
	partition_logic(polygon, temp_lr, temp_rl, begin, end, start_point, end_point, flag);
	//call the recursive functions to solve for the left and right part of the problem
	recursive_partioning(polygon, 0, temp_rl);
	recursive_partioning(polygon, temp_rl, polygon->numVertices);
}

//recursive function to solve the problem by the means of divide and conquer
void recursive_partioning(Polygon *polygon, int begin, int end){
	if(end > begin + 1){
		pair<double,double> start_point = polygon->coordinates[begin];
		pair<double,double> end_point = (end == polygon->numVertices) ? polygon->coordinates[0] : polygon->coordinates[end];
		pair<double,double> random_start_point = random_point_segment(start_point, end_point);
		int random_index = (rand()%(end - begin - 1)) + begin + 1;
		pair<double,double> random_end_point = polygon->coordinates[random_index];
		swap(polygon, begin + 1, random_index);
		int temp_lr = begin + 2, temp_rl = end - 1;
		bool flag = to_left(random_start_point, random_end_point, polygon->coordinates[begin]);
		partition_logic(polygon, temp_lr, temp_rl, begin, end, random_start_point, random_end_point, flag);
		//call the recursive functions to solve for the left and right part of the problem
		recursive_partioning(polygon, begin, temp_rl);
		recursive_partioning(polygon, temp_rl, end);
	}
}

//helper function for partitioning
static void partition_logic(Polygon *polygon, int &temp_lr, int &temp_rl, int &begin, int &end, pair<double, double> start_point, pair<double, double> end_point, int flag){
	while(temp_lr <= temp_rl){
		if(DEBUG){
			cout << "temp_lr = " << temp_lr << " temp_rl = " << temp_rl << endl;
		}
		while(temp_lr < end && to_left(start_point, end_point, polygon->coordinates[temp_lr]) == flag){
			temp_lr++;
		}
		while(temp_rl > begin + 1 && to_left(start_point, end_point, polygon->coordinates[temp_rl]) != flag){
			temp_rl--;
		}
		if(temp_lr <= temp_rl){
			swap(polygon, temp_lr, temp_rl);
			temp_lr++;
			temp_rl--;
		}
	}
	swap(polygon, begin + 1, temp_rl);
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
void random_points(Polygon *polygon, double min, double max){
	for (int i = 0; i < polygon->numVertices; i++){
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