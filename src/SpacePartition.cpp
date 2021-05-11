#include<iostream>
#include"Polygon.hpp"

#define DEBUG 0
using namespace std;
extern double timer;

static void random_points(Polygon *polygon, double min, double max); // Initialises the coordinate vector with num_vertices number of random points
static pair<double,double> random_point_segment(const pair<double,double> &start_point, const pair<double,double> &end_point); // Returns a random point on the given line segment
static bool to_left(const pair<double,double> &start_point, const pair<double,double> &end_point, const pair<double,double> &test_point); // Returns a value based on which side the test point is w.r.t the line
static void swap(Polygon *polygon, unsigned int first_index, unsigned int second_index); // Exchanges contents of the said indices in the vector
static void recursive_partioning(Polygon *polygon, unsigned int begin, unsigned int end); // Recursive function to solve the problem by the means of divide and conquer
static void partition_logic(Polygon *polygon, unsigned int &temp_lr, unsigned int &temp_rl, unsigned int &begin, unsigned int &end, pair<double, double> start_point, pair<double, double> end_point, int flag); // Splits the vector into two parts (left and right) based on the random end_point generated
static double random_num(); // Generates random number between 0 and 1

// Implementation of the space partition algorithm to generate random polygons
void space_partition(Polygon *polygon, int min, int max, bool verbose){
	start_timer(start);
	random_points(polygon, min, max);
	int end_index = rand()%(polygon->numVertices - 1) + 1; // Choose the start point as the first element and any random point as the initial final element
	pair<double, double> start_point = polygon->coordinates[0], end_point = polygon->coordinates[end_index]; // Note the start and end points
	swap(polygon, 1, end_index); // Exchange the 2nd element with the randomly chosen last element
	unsigned int begin = 0, end = polygon->numVertices; // Choose the begin and end of the vector that is to be split. (initially it is the entire vector)
	unsigned int temp_lr = 2, temp_rl = polygon->numVertices - 1; // Traversal indices from left and right are initialised
	int flag = 1;
	partition_logic(polygon, temp_lr, temp_rl, begin, end, start_point, end_point, flag); // Split the vector two left sub vector and right subvector
	recursive_partioning(polygon, begin, temp_rl); // Solve for the left sub vector  
	recursive_partioning(polygon, temp_rl, end); // Solve for the right sub vector
	end_timer(start, timer);
	if(verbose) printf("Number of vertices = %3u,  Lower bound = %4d, Upper bound = %4d | Time taken for generation = %lf s\n", polygon->numVertices, min, max, timer);
}

// Recursive function to solve the problem by the means of divide and conquer
void recursive_partioning(Polygon *polygon, unsigned int begin, unsigned int end){
	if(end > begin + 1){
		pair<double,double> start_point = polygon->coordinates[begin]; // Create start and end points of this sub vector
		pair<double,double> end_point = (end == polygon->numVertices) ? polygon->coordinates[0] : polygon->coordinates[end];
		pair<double,double> random_start_point = random_point_segment(start_point, end_point);
		int random_index = (rand()%(end - begin - 1)) + begin + 1;
		pair<double,double> random_end_point = polygon->coordinates[random_index];
		swap(polygon, begin + 1, random_index);
		unsigned int temp_lr = begin + 2, temp_rl = end - 1; // Traversal indices from left and right are initialised
		bool flag = to_left(random_start_point, random_end_point, polygon->coordinates[begin]);
		partition_logic(polygon, temp_lr, temp_rl, begin, end, random_start_point, random_end_point, flag); // Split this sub vector further into two sub vectors	 
		recursive_partioning(polygon, begin, temp_rl); // Solve for the left sub vector
		recursive_partioning(polygon, temp_rl, end); // Solve for the right sub vector
	}
}

// Splits the vector into two parts (left and right) based on the random end_point generated
static void partition_logic(Polygon *polygon, unsigned int &temp_lr, unsigned int &temp_rl, unsigned int &begin, unsigned int &end, 
	pair<double, double> start_point, pair<double, double> end_point, int flag){
	while(temp_lr <= temp_rl){ // As long as left_to_right index is less than right_to_left index
		if(DEBUG) cout << "Before => temp_lr = " << temp_lr << " temp_rl = " << temp_rl << endl;
		
		// Find a point from the left side of the vector such that it CANNOT be traversed in a CW manner
		while(temp_lr < end && to_left(start_point, end_point, polygon->coordinates[temp_lr]) == flag) temp_lr++;
		
		// Find a point from the right side of the vector such that it CANNOT be traversed in a CW manner
		while(temp_rl > begin + 1 && to_left(start_point, end_point, polygon->coordinates[temp_rl]) != flag) temp_rl--;
	
		if(temp_lr <= temp_rl){
			swap(polygon, temp_lr, temp_rl);
			temp_lr++;
			temp_rl--;
		}
		if(DEBUG) cout << "After => temp_lr = " << temp_lr << " temp_rl = " << temp_rl << endl;	
	}
	swap(polygon, begin + 1, temp_rl);
}

// Exchanges contents of the said indices in the vector
void swap(Polygon *polygon, unsigned int first_index, unsigned int second_index){
	if(DEBUG) cout << "Before swapping " << polygon->coordinates[first_index].first << " " << polygon->coordinates[first_index].second << " " << polygon->coordinates[second_index].first << " " << polygon->coordinates[second_index].second << endl;
	
	// Using in-built swap function to swap the tuples
	polygon->coordinates[second_index].swap(polygon->coordinates[first_index]);
	if(DEBUG) cout << "After swapping " << polygon->coordinates[first_index].first << " " << polygon->coordinates[first_index].second << " " << polygon->coordinates[second_index].first << " " << polygon->coordinates[second_index].second << endl;
}

// This function returns a value based on which side the test point is w.r.t the line
bool to_left(const pair<double,double> &start_point, const pair<double,double> &end_point, const pair<double,double> &test_point){
	if(DEBUG){
		int ret = (end_point.first - start_point.first) * (test_point.second - start_point.second) <= (end_point.second - start_point.second) * (test_point.first - start_point.first);
		cout << "Start point " << start_point.first << " " << start_point.second << endl;
		cout << "End point " << end_point.first << " " << end_point.second << endl;
		cout << "Test point " << test_point.first << " " << test_point.second << endl;
		cout << "Distance " << (end_point.first - start_point.first) * (test_point.second - start_point.second) - (end_point.second - start_point.second) * (test_point.first - start_point.first) << " ";
		cout << "Return value " << ret << endl;
	}
	return (end_point.first - start_point.first) * (test_point.second - start_point.second) <= (end_point.second - start_point.second) * (test_point.first - start_point.first);
}

// Returns a random point on the line segment
pair<double,double> random_point_segment(const pair<double,double> &start_point, const pair<double,double> &end_point){
	double random_ratio = random_num();
	pair<double,double> random_point;
	random_point.first = start_point.first + (end_point.first - start_point.first) * random_ratio;
	random_point.second = start_point.second + (end_point.second - start_point.second) * random_ratio;  
	return random_point;
}

// Generates a random set of points from which a random polygon is constructed
void random_points(Polygon *polygon, double min, double max){
	for (unsigned i = 0; i < polygon->numVertices; i++){
		polygon->coordinates.push_back(pair<double,double>(min + (max - min) * random_num(), min + (max - min) * random_num()));
		if(DEBUG) cout << "Random point " << polygon->coordinates[i].first << " " << polygon->coordinates[i].second << endl;
	}
}

// Generates a random number between 0 and 1
double random_num(){
	return rand()/(double)RAND_MAX;
}