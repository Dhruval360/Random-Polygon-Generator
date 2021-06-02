#include<iostream>
#include<random>
#include"Polygon.hpp"

using namespace std;
// Uncomment to enable debug 
//#define DEBUG 

extern double timer;

// Generates random number between 0 and 1
static default_random_engine generator(clock());
static uniform_real_distribution<double> random_ratio(0, 1);

// Return a random point on the given line segment
static pair<double,double> random_point_segment(const pair<double,double> &start_point, const pair<double,double> &end_point);

// This function returns a value based on which side the test point is w.r.t the line
static bool to_left(const pair<double,double> &start_point, const pair<double,double> &end_point, const pair<double,double> &test_point);

// Exchanges contents of the said indices in the vector
static void swap(Polygon *polygon, unsigned int first_index, unsigned int second_index);

// Recursive function to solve the problem by the means of divide and conquer
static void recursive_partioning(Polygon *polygon, unsigned int begin, unsigned int end);

// Splits the vector into two parts (left and right) based on the random end_point generated
static void partition_logic(Polygon *polygon, unsigned int &temp_lr, unsigned int &temp_rl, unsigned int &begin, unsigned int &end, pair<double, double> &start_point, pair<double, double> &end_point, int flag);

// Implementation of the space partition algorithm to generate random polygons
void spacePartition(Polygon *polygon, bool verbose){
	start_timer(start);
	// Choose the start point as the first element and any random point as the initial final element
	int end_index = rand()%(polygon->numVertices - 1) + 1; 
	
	// Note the start and end points
	pair<double, double> start_point = polygon->coordinates[0], end_point = polygon->coordinates[end_index]; 
	
	// Exchange the 2nd element with the randomly chosen last element
	swap(polygon, 1, end_index); 
	
	// Choose the begin and end of the vector that is to be split. (initially it is the entire vector)
	unsigned int begin = 0, end = polygon->numVertices; 
	
	// Traversal indices from left and right are initialised
	unsigned int temp_lr = 2, temp_rl = polygon->numVertices - 1; 
	int flag = 1;
	
	// Split the vector two left sub vector and right subvector
	partition_logic(polygon, temp_lr, temp_rl, begin, end, start_point, end_point, flag); 
	
	// Solve for the left sub vector 
	recursive_partioning(polygon, begin, temp_rl);
	
	// Solve for the right sub vector  
	recursive_partioning(polygon, temp_rl, end); 
	end_timer(start, timer);
	if(verbose) printf("Number of vertices = %3u | Time taken for generation = %lf s\n", polygon->numVertices, timer);
}

// Recursive function to solve the problem by the means of divide and conquer
void recursive_partioning(Polygon *polygon, unsigned int begin, unsigned int end){
	if(end > begin + 1){
		// Create start and end points of this sub vector
		pair<double,double> start_point = polygon->coordinates[begin]; 
		pair<double,double> end_point = (end == polygon->numVertices) ? polygon->coordinates[0] : polygon->coordinates[end];
		pair<double,double> random_start_point = random_point_segment(start_point, end_point);
		int random_index = (rand()%(end - begin - 1)) + begin + 1;
		pair<double,double> random_end_point = polygon->coordinates[random_index];
		swap(polygon, begin + 1, random_index);
		
		// Traversal indices from left and right are initialised
		unsigned int temp_lr = begin + 2, temp_rl = end - 1; 
		bool flag = to_left(random_start_point, random_end_point, polygon->coordinates[begin]);
		
		// Split this sub vector further into two sub vectors	
		partition_logic(polygon, temp_lr, temp_rl, begin, end, random_start_point, random_end_point, flag);  
		
		// Solve for the left sub vector
		recursive_partioning(polygon, begin, temp_rl);
		
		// Solve for the right sub vector 
		recursive_partioning(polygon, temp_rl, end); 
	}	
}

// Splits the vector into two parts (left and right) based on the random end_point generated
static void partition_logic(Polygon *polygon, unsigned int &temp_lr, unsigned int &temp_rl, unsigned int &begin, unsigned int &end, 
	pair<double, double> &start_point, pair<double, double> &end_point, int flag){
	while(temp_lr <= temp_rl){ // As long as left_to_right index is less than right_to_left index
		#ifdef DEBUG 
			cout << "Before => temp_lr = " << temp_lr << " temp_rl = " << temp_rl << '\n';
		#endif
		
		// Find a point from the left side of the vector such that it CANNOT be traversed in a CW manner
		while(temp_lr < end && to_left(start_point, end_point, polygon->coordinates[temp_lr]) == flag) temp_lr++;
		
		// Find a point from the right side of the vector such that it CANNOT be traversed in a CW manner
		while(temp_rl > begin + 1 && to_left(start_point, end_point, polygon->coordinates[temp_rl]) != flag) temp_rl--;
	
		if(temp_lr <= temp_rl){
			swap(polygon, temp_lr, temp_rl);
			temp_lr++;
			temp_rl--;
		}
		#ifdef DEBUG 
			cout << "After => temp_lr = " << temp_lr << " temp_rl = " << temp_rl << '\n';	
		#endif
	}
	swap(polygon, begin + 1, temp_rl);
}

// Exchanges contents of the said indices in the vector
void swap(Polygon *polygon, unsigned int first_index, unsigned int second_index){
	#ifdef DEBUG 
		cout << "Before swapping " << polygon->coordinates[first_index].first << " " << polygon->coordinates[first_index].second << " " << polygon->coordinates[second_index].first << " " << polygon->coordinates[second_index].second << '\n';
	#endif

	// Using in-built swap function to swap the tuples
	polygon->coordinates[second_index].swap(polygon->coordinates[first_index]);
	
	#ifdef DEBUG 
		cout << "After swapping " << polygon->coordinates[first_index].first << " " << polygon->coordinates[first_index].second << " " << polygon->coordinates[second_index].first << " " << polygon->coordinates[second_index].second << '\n';
	#endif
}

// This function returns a value based on which side the test point is w.r.t the line
bool to_left(const pair<double,double> &start_point, const pair<double,double> &end_point, const pair<double,double> &test_point){
	#ifdef DEBUG
		int ret = (end_point.first - start_point.first) * (test_point.second - start_point.second) <= (end_point.second - start_point.second) * (test_point.first - start_point.first);
		cout << "Start point " << start_point.first << " " << start_point.second << '\n';
		cout << "End point " << end_point.first << " " << end_point.second << '\n';
		cout << "Test point " << test_point.first << " " << test_point.second << '\n';
		cout << "Distance " << (end_point.first - start_point.first) * (test_point.second - start_point.second) - (end_point.second - start_point.second) * (test_point.first - start_point.first) << " ";
		cout << "Return value " << ret << '\n';
	#endif
	return (end_point.first - start_point.first) * (test_point.second - start_point.second) <= (end_point.second - start_point.second) * (test_point.first - start_point.first);
}

// Returns a random point on the line segment
pair<double,double> random_point_segment(const pair<double,double> &start_point, const pair<double,double> &end_point){
	double random_ratio_ = random_ratio(generator);
	pair<double,double> random_point;
	random_point.first = start_point.first + (end_point.first - start_point.first) * random_ratio_;
	random_point.second = start_point.second + (end_point.second - start_point.second) * random_ratio_;  
	return random_point;
}