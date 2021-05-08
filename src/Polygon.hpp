#include<utility>
#include<vector>

using namespace std;

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
        vector<pair<double, double>> coordinates;

        Polygon(){}

        Polygon(unsigned int numVertices);

        void Generator1(bool verbose);
        void Generator2();
        void Generator3();
};

void polarGenerator(double x, double y, double averageRadius, double irregularity, double spike, Polygon *p, bool verbose);
void write(Polygon *p, unsigned num);