#include<stdio.h>
#include<stdlib.h>
#include<utility>
#include<vector>
using namespace std;
class Polygon;

void polygonGenerator(double x, double y, double averageRadius, double irregularity, double spike, Polygon *p);
void write(Polygon *p, unsigned num);

double randf();
void random_points(Polygon *ps, int count, double min, double max);
pair<double,double> random_point_on_segment(const pair<double,double> &s, const pair<double,double> &e);
bool to_left(const pair<double,double> &s, const pair<double,double> &e, const pair<double,double> &p);
void swap(Polygon *ps, int i, int j);
void space_partion_rec(Polygon *ps, int count, int l, int r);
void space_partition(Polygon *ps, int count, int min, int max);

class Polygon{
    public:
        unsigned int numVertices;
        //pair<double, double> *coordinates;
        vector<pair<double, double>> coordinates;

        Polygon(){}

        Polygon(unsigned int numVertices){
            this->numVertices = numVertices;
        }

        ~Polygon(){ }

        void Generator1(double x, double y, double averageRadius, double irregularity, double spike){
            polygonGenerator(x, y, averageRadius, irregularity, spike, this);
        };
        void Generator2(int count, int min, int max){
            space_partition(this, count, min, max);
        }
        void Generator3();
};