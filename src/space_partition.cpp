#include<iostream>
#include<fstream>
#include<random>
#include<cstring>
#include"polygon.hpp"
#define DEBUG 1
using namespace std;

void space_partition(point points_array[], int num_vertices){
	//choose the start point as the first element and any random point as the initial final element
	int end_index = rand() % (num_vertices - 1) + 1;
	point start_point = points_array[0], end_point = points_array[end_index];
	//exchange the 2nd element with the randomly chosen last element
	swap(points_array, 1, end_index);
	int temp_lr = 2;
	int temp_rl = num_vertices - 1;
	partion_logic(points_array, temp_lr, temp_rl, 0, num_vertices, start_point, end_point, 1);
	recursive_partitioning(points_array, num_vertices, 0, temp_rl);
	recursive_partitioning(points_array, num_vertices, temp_rl, num_vertices);
}

void recursive_partitioning(point points_array[], int num_vertices, int begin, int end){
	if(end > begin + 1){
		int rp = rand() % (end - begin - 1) + begin + 1;
		point start_point = points_array[begin];
		point end_point = end == num_vertices ? points_array[0] : points_array[end];
		point rps = random_point_on_segment(start_point, end_point);
		point rpe = points_array[rp];
		swap(points_array, begin + 1, rp);
		int temp_lr = begin + 2;
		int temp_rl = end - 1;
		bool l_dir = to_left(rps, rpe, points_array[begin]);
		partion_logic(points_array, temp_lr, temp_rl, begin, end, rps, rpe, l_dir);
		recursive_partitioning(points_array, num_vertices, begin, temp_rl);
		recursive_partitioning(points_array, num_vertices, temp_rl, end);
	}
}

//exchanges contents of the said indices in the array
void swap(point points_array[], int first_index, int second_index){
	point temp_point = points_array[first_index];
	points_array[first_index] = points_array[second_index];
	points_array[second_index] = temp_point;
}

bool to_left(const point &start_point, const point &end_point, const point &p){
	if(DEBUG){
		cout << "Start point " << start_point.x << " " << start_point.y << endl;
		cout << "End point " << end_point.x << " " << end_point.y << endl;
		cout << "Test point " << p.x << " " << p.y << endl;
		cout << "Distance " << (end_point.x - start_point.x) * (p.y - start_point.y) - (end_point.y - start_point.y) * (p.x - start_point.x) << endl; 
	}
	return (end_point.x - start_point.x) * (p.y - start_point.y) <= (end_point.y - start_point.y) * (p.x - start_point.x);
}

//returns a random point on the line segment
point random_point_on_segment(const point &start_point, const point &end_point){
	double random_ratio = randf();
	double random_x = start_point.x + (end_point.x - start_point.x) * random_ratio;
	double random_y = start_point.y + (end_point.y - start_point.y) * random_ratio;  
	return point(random_x, random_y);
}

//generates a random set of points from which a random polygon is constructed
void random_points(point points_array[], int num_vertices, double min, double max){
	for (int i = 0; i < num_vertices; i++){
		points_array[i] = point(min + (max - min) * randf(), min + (max - min) * randf());
		if(DEBUG){
			cout << "Random point " << points_array[i].x << " " << points_array[i].y << endl;
		}
	}
}

//generates a random number between 0 and 1
double randf(){
	return rand()/(double)RAND_MAX;
}

void partion_logic(point points_array[], int temp_lr, int temp_rl, int begin, int end, point rps, point rpe, bool flag){
	while(temp_lr <= temp_rl){
		if(DEBUG){
			cout << "temp_lr = " << temp_lr << " temp_rl = " << temp_rl << endl;
		}
		while(temp_lr < end && to_left(rps, rpe, points_array[temp_lr]) == flag){
			temp_lr++;
		}
		while(temp_rl > begin + 1 && to_left(rps, rpe, points_array[temp_rl]) != flag){
			temp_rl--;
		}
		if(temp_lr <= temp_rl){
			swap(points_array, temp_lr, temp_rl);
			temp_lr++;
			temp_rl--;
		}
	}
	swap(points_array, begin + 1, temp_rl);
}

//writes the generated results into a wkt file
void write_to_wkt(point points_array[], int num_vertices, char *filename){
	ofstream file(filename, ofstream::out);
	for(int i = 0; i < num_vertices; i++){
		if(i == 0){
			file << "POLYGON ((";
		}
		else if(i == num_vertices-1){
			file << points_array[i].x << " " << points_array[i].y << ", ";
			file << points_array[0].x << " " << points_array[0].y << "))" << endl;
			if(DEBUG){
				cout << "Polygon successfully written to file" << endl;
			}
			return;
		}
		file << points_array[i].x << " " << points_array[i].y << ", ";
	}
}