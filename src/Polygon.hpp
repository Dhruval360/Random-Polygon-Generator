#include<utility>
#include<vector>

using namespace std;

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