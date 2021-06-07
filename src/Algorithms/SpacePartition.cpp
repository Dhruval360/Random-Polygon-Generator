#include <random>
#include <iostream>
#include "../Classes/Classes.hpp"

// Uncomment the line below to enable debug 
//#define DEBUG 1
using namespace std;

extern double timer;

// Generates random number between 0 and 1
static default_random_engine generator(clock());
static uniform_real_distribution<double> random_ratio(0, 1);

// Return a random point on the given line segment
static pair<double,double> random_point_segment(const pair<double,double> &start_point, const pair<double,double> &end_point);

// This function returns a value based on which side the test point is w.r.t the line
static bool to_left(const pair<double,double> &start_point, const pair<double,double> &end_point, const pair<double,double> &test_point);

// Recursive function to solve the problem by the means of divide and conquer
static vector<pair<double,double>> recursive_partioning(pair<double,double> p1, pair<double,double> p2, vector<pair<double,double>> &points_vector);

// Returns a new point in a subvector and deletes the element
static pair<double,double> get_new_point(vector<pair<double,double>> &sub_vector);

// Implementation of the space partition algorithm to generate random polygons
void spacePartition(Polygon *polygon, bool verbose){
	start_timer(start);
	vector<pair<double,double>> points_vector = polygon->coordinates;
	#ifdef DEBUG
		cout << "Points received:\n[ ";
		for(int i = 0; i < points_vector.size(); ++i)
			cout << points_vector[i] << ' ';
		cout << "]\n" ;
	#endif
	
	// Get two points from vector as initial and final points
	pair<double,double> initial_point = get_new_point(points_vector), final_point = get_new_point(points_vector);
	vector<pair<double,double>> sub_vector_1, sub_vector_2;
	
	// Partition the vector into two more vectors
	for (pair<double,double> point : points_vector){
		if (to_left(initial_point, final_point, point)){
			sub_vector_1.push_back(point);
		} 
		else{
			sub_vector_2.push_back(point);
		}
	}
	#ifdef DEBUG
		cout << "\nGroup 1:\n[ ";
		for(int i = 0; i < sub_vector_1.size(); ++i)
			cout << sub_vector_1[i] << ' ';

		cout <<  "]\nGroup 2:\n[ ";
		for(int i = 0; i < sub_vector_2.size(); ++i)
			cout << sub_vector_2[i] << ' ';
		cout << "]\n\n";
	#endif

	// Call recursive functions to solve for left part and right part into hull 1 and hull 2
	vector<pair<double,double>> hull_1 = recursive_partioning(initial_point, final_point, sub_vector_1);
	vector<pair<double,double>> hull_2 = recursive_partioning(final_point, initial_point, sub_vector_2);
	#ifdef DEBUG
		cout << "\nHull 1:\n[ ";
		for(int i = 0; i < hull_1.size(); ++i)
			cout << hull_1[i] << ' ';

		cout << "]\nHull 2:\n[ ";
		for(int i = 0;i < hull_2.size();++i)
			cout << hull_2[i] << ' ';		
		cout << "]\n\n";
	#endif
	
	// Merge hull 1 and hull 2 to form final set of points
	vector<pair<double,double>> final_vector;
	final_vector.push_back(initial_point);
	final_vector.insert(final_vector.end(), hull_1.begin(), hull_1.end());
	final_vector.push_back(final_point);
	final_vector.insert(final_vector.end(), hull_2.begin(), hull_2.end());

	polygon->coordinates = final_vector;
	end_timer(start, timer);
	if(verbose) printf("Number of vertices = %3u | Time taken for generation = %lf s\n", polygon->numVertices, timer);
}

// Recursive function to solve the problem by the means of divide and conquer
static vector<pair<double,double>> recursive_partioning(pair<double,double> start_point, pair<double,double> end_point, vector<pair<double,double>> &points_vector){
	
	// Recursion continues if sub vector not empty
	if(points_vector.size() != 0){		
		// Get new point and obtain a random point on the line segment start point-end point
		pair<double,double> new_point = get_new_point(points_vector);
		pair<double,double> random_point = random_point_segment(start_point, end_point);
		#ifdef DEBUG
			cout << "Random point in array " << new_point << '\n';
			cout << "Random point on line start_point - end_point " << random_point << '\n';
		#endif
		
		// Partition the vector into two more vectors 
		vector<pair<double,double>> sub_vector_1, sub_vector_2, merged_vector;
		for (pair<double,double> point : points_vector){
			if (to_left(random_point, new_point, point)) sub_vector_1.push_back(point);
			else sub_vector_2.push_back(point);
		}

		// If the start point is to the left of the line segment
		if (to_left(random_point, new_point, start_point)){
			vector<pair<double,double>> left_part = recursive_partioning(start_point, new_point, sub_vector_1);
			merged_vector.insert(merged_vector.end(), left_part.begin(), left_part.end());
			merged_vector.push_back(new_point);
			vector<pair<double,double>> right_part = recursive_partioning(new_point, end_point, sub_vector_2);
			merged_vector.insert(merged_vector.end(), right_part.begin(), right_part.end());
		} 
		// If the start point is to the right of the line segment
		else{
			vector<pair<double,double>> right_part = recursive_partioning(start_point, new_point, sub_vector_2);
			merged_vector.insert(merged_vector.end(), right_part.begin(), right_part.end());
			merged_vector.push_back(new_point);
			vector<pair<double,double>> left_part = recursive_partioning(new_point, end_point, sub_vector_1);
			merged_vector.insert(merged_vector.end(), left_part.begin(), left_part.end());
		}
		return merged_vector;
	}
	// Terminate when sub-vector is empty
	else return points_vector;
}

// This function returns a value based on which side the test point is w.r.t the line
static bool to_left(const pair<double,double> &start_point, const pair<double,double> &end_point, const pair<double,double> &test_point){
	#ifdef DEBUG
		int ret = (end_point.first - start_point.first) * (test_point.second - start_point.second) <= (end_point.second - start_point.second) * (test_point.first - start_point.first);
		cout << "\nStart point " << start_point.first << " " << start_point.second << '\n'
		     << "End point " << end_point.first << " " << end_point.second << '\n'
		     << "Test point " << test_point.first << " " << test_point.second << '\n'
		     << "Distance " << (end_point.first - start_point.first) * (test_point.second - start_point.second) - (end_point.second - start_point.second) * (test_point.first - start_point.first) << " "
		     << "Return value " << ret << '\n';
	#endif
	return (end_point.first - start_point.first) * (test_point.second - start_point.second) <= (end_point.second - start_point.second) * (test_point.first - start_point.first);
}

// Returns a new point in a subvector and deletes the element
static pair<double,double> get_new_point(vector<pair<double,double>> &sub_vector){
	pair<double,double> new_point = sub_vector[0];
	sub_vector.erase(sub_vector.begin());
	return new_point;
}

// Returns a random point on the line segment
pair<double,double> random_point_segment(const pair<double,double> &start_point, const pair<double,double> &end_point){
	double random_ratio_ = random_ratio(generator);
	pair<double,double> random_point;
	random_point.first = start_point.first + (end_point.first - start_point.first) * random_ratio_;
	random_point.second = start_point.second + (end_point.second - start_point.second) * random_ratio_;  
	return random_point;
}