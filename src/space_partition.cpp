#include<iostream>
#include<fstream>
#include<random>
#include<cstring>
#include "polygon.hpp"
#define DEBUG 1
using namespace std;

double randf();
void random_points(Polygon &ps, int count, double min, double max);
pair<double,double> random_point_on_segment(const pair<double,double> &s, const pair<double,double> &e);
bool to_left(const pair<double,double> &s, const pair<double,double> &e, const pair<double,double> &p);
void swap(Polygon &ps, int i, int j);
void space_partion_rec(Polygon &ps, int count, int l, int r);
void space_partition(Polygon &ps, int count);
//writes the randomly generated points to the wkt file
void write_to_wkt(Polygon &ps, int count, char *filename);
//error message
const char *usage = "Usage: ./a.out [count] [min] [max] [output_file]";

int main(int argc, char *argv[]){
	srand(time(NULL));
	if (argc != 5){
		cout << usage << endl;
		return -1;
	}
	int count = atoi(argv[1]);
	double min = atof(argv[2]);
	double max = atof(argv[3]);
	Polygon *ps = new Polygon[1];
	ps[0] = Polygon(count);
	random_points(ps[0], count, min, max);
	space_partition(ps[0], count);
	write(ps,1);
	//write_to_wkt(ps[0], count, argv[4]);
	//delete[] ps;
	return 0;
}

void space_partition(Polygon &ps, int count){
	//choose the start point as the first element and any random point as the initial final element
	int end_index = rand() % (count - 1) + 1;
	pair<double, double> start_point = ps.coordinates[0], end_point = ps.coordinates[end_index];
	//exchange the 2nd element with the randomly chosen last element
	swap(ps, 1, end_index);
	int temp_lr = 2;
	int temp_rl = count - 1;
	while(temp_lr <= temp_rl){
		if(DEBUG){
			cout << "temp_lr = " << temp_lr << " temp_rl = " << temp_rl << endl;
		}
		while(temp_lr < count && to_left(start_point, end_point, ps.coordinates[temp_lr])){
			temp_lr++;
		}
		while(temp_rl > 1 && !to_left(start_point, end_point, ps.coordinates[temp_rl])){
			temp_rl--;
		}
		if(temp_lr <= temp_rl){
			swap(ps, temp_lr, temp_rl);
			temp_lr++;
			temp_rl--;
		}
	}
	swap(ps, 1, temp_rl);
	space_partion_rec(ps, count, 0, temp_rl);
	space_partion_rec(ps, count, temp_rl, count);
}

void space_partion_rec(Polygon &ps, int count, int begin, int end){
	if(end > begin + 1){
		int rp = rand() % (end - begin - 1) + begin + 1;
		pair<double,double> start_point = ps.coordinates[begin];
		pair<double,double> end_point = end == count ? ps.coordinates[0] : ps.coordinates[end];
		pair<double,double> rps = random_point_on_segment(start_point, end_point);
		pair<double,double> rpe = ps.coordinates[rp];
		swap(ps, begin + 1, rp);
		int temp_lr = begin + 2;
		int temp_rl = end - 1;
		bool l_dir = to_left(rps, rpe, ps.coordinates[begin]);
		while(temp_lr <= temp_rl){
			while(temp_lr < end && to_left(rps, rpe, ps.coordinates[temp_lr]) == l_dir){
				temp_lr++;
			}
			while(temp_rl > begin + 1 && to_left(rps, rpe, ps.coordinates[temp_rl]) != l_dir){
				temp_rl--;
			}
			if(temp_lr <= temp_rl){
				swap(ps, temp_lr, temp_rl);
				temp_lr++;
				temp_rl--;
			}
		}
		swap(ps, begin + 1, temp_rl);
		space_partion_rec(ps, count, begin, temp_rl);
		space_partion_rec(ps, count, temp_rl, end);
	}
}

//exchanges contents of the said indices in the array
void swap(Polygon &ps, int first_index, int second_index){
	pair<double, double> temp_point = ps.coordinates[first_index];
	ps.coordinates[first_index] = ps.coordinates[second_index];
	ps.coordinates[second_index] = temp_point;
}

bool to_left(const pair<double,double> &start_point, const pair<double,double> &end_point, const pair<double,double> &p){
	if(DEBUG){
		cout << "Start point " << start_point.first << " " << start_point.second << endl;
		cout << "End point " << end_point.first << " " << end_point.second << endl;
		cout << "Test point " << p.first << " " << p.second << endl;
		cout << "Distance " << (end_point.first - start_point.first) * (p.second - start_point.second) - (end_point.second - start_point.second) * (p.first - start_point.first) << endl; 
	}
	return (end_point.first - start_point.first) * (p.second - start_point.second) <= (end_point.second - start_point.second) * (p.first - start_point.first);
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
void random_points(Polygon &ps, int count, double min, double max){
	for (int i = 0; i < count; i++){
		ps.coordinates[i].first = min + (max - min) * randf();
		ps.coordinates[i].second = min + (max - min) * randf();
		if(DEBUG){
			cout << "Random point " << ps.coordinates[i].first << " " << ps.coordinates[i].second << endl;
		}
	}
}

//generates a random number between 0 and 1
double randf(){
	return rand()/(double)RAND_MAX;
}

//writes the generated results into a wkt file
void write_to_wkt(Polygon &ps, int count, char *filename){
	ofstream file(filename, ofstream::out);
	for(int i = 0; i < count; i++){
		if(i == 0){
			file << "POLYGON ((";
		}
		else if(i == count-1){
			file << ps.coordinates[i].first << " " << ps.coordinates[i].second << ", ";
			file << ps.coordinates[0].first << " " << ps.coordinates[0].second << "))" << endl;
			if(DEBUG){
				cout << "Polygon successfully written to file" << endl;
			}
			return;
		}
		file << ps.coordinates[i].first << " " << ps.coordinates[i].second << ", ";
	}
}