#include<iostream>
#include<fstream>
#include<random>
#include<cstring>
#include<utility>
#include "Polygon.hpp"
#define DEBUG 1
using namespace std;
double randf();
void random_points(Polygon *ps, int count, double min, double max);
pair<double,double> random_point_on_segment(const pair<double,double> &s, const pair<double,double> &e);
bool to_left(const pair<double,double> &s, const pair<double,double> &e, const pair<double,double> &p);
void swap(Polygon *ps, int i, int j);
void space_partion_rec(Polygon *ps, int count, int l, int r);
void space_partition(Polygon *ps, int count, int min, int max);
//writes the randomly generated points to the wkt file
void write_to_wkt(Polygon *ps, int count, char *filename);

int main(int argc, char *argv[]){
	srand(time(NULL));
	if (argc != 5){
		return -1;
	}
	int count = atoi(argv[1]);
	double min = atof(argv[2]);
	double max = atof(argv[3]);
	Polygon *ps = new Polygon[1];
	ps[0] = Polygon(count);
	ps[0].Generator2(count, min, max);
	//random_points(ps, count, min, max);
	//space_partition(ps, count);
	//write(ps,1);
	write_to_wkt(ps, count, argv[4]);
	//delete[] ps;
	return 0;
}

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
	space_partion_rec(polygon, num_vertices, 0, temp_rl);
	space_partion_rec(polygon, num_vertices, temp_rl, num_vertices);
}

void space_partion_rec(Polygon *polygon, int num_vertices, int begin, int end){
	if(end > begin + 1){
		pair<double,double> start_point = polygon->coordinates[begin];
		pair<double,double> end_point = (end == num_vertices) ? polygon->coordinates[0] : polygon->coordinates[end];
		pair<double,double> random_start_point = random_point_on_segment(start_point, end_point);
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
		space_partion_rec(polygon, num_vertices, begin, temp_rl);
		space_partion_rec(polygon, num_vertices, temp_rl, end);
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
pair<double,double> random_point_on_segment(const pair<double,double> &start_point, const pair<double,double> &end_point){
	double random_ratio = randf();
	pair<double,double> random_point;
	random_point.first = start_point.first + (end_point.first - start_point.first) * random_ratio;
	random_point.second = start_point.second + (end_point.second - start_point.second) * random_ratio;  
	return random_point;
}

//generates a random set of points from which a random polygon is constructed
void random_points(Polygon *polygon, int num_vertices, double min, double max){
	for (int i = 0; i < num_vertices; i++){
		polygon->coordinates.push_back(pair<double,double>(min + (max - min) * randf(), min + (max - min) * randf()));
		if(DEBUG){
			cout << "Random point " << polygon->coordinates[i].first << " " << polygon->coordinates[i].second << endl;
		}
	}
}

//generates a random number between 0 and 1
double randf(){
	return rand()/(double)RAND_MAX;
}

//writes the generated results into a wkt file
void write_to_wkt(Polygon *ps, int count, char *filename){
	ofstream file(filename, ofstream::out);
	for(int i = 0; i < count; i++){
		if(i == 0){
			file << "POLYGON ((";
		}
		else if(i == count-1){
			file << ps->coordinates[i].first << " " << ps->coordinates[i].second << ", ";
			file << ps->coordinates[0].first << " " << ps->coordinates[0].second << "))" << endl;
			if(DEBUG){
				cout << "Polygon successfully written to file" << endl;
			}
			return;
		}
		file << ps->coordinates[i].first << " " << ps->coordinates[i].second << ", ";
	}
}