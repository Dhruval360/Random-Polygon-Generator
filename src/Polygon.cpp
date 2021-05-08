#include<random>
#include"Polygon.hpp"
using namespace std;

Polygon::Polygon(unsigned int numVertices){
    this->numVertices = numVertices;
    this->coordinates.reserve(numVertices);
}

void Polygon::Generator1(bool verbose){
    static default_random_engine generator(clock());
    static uniform_real_distribution<double> circleCentresDistribution(-500, 500); 
    static uniform_real_distribution<double> avgRadiusDistribution(50, 500);
    static uniform_real_distribution<double> angleIrregularityDistribution(0.0, 1.0);
    static uniform_real_distribution<double> spikeDistribution(0.0, 55.0);
    polarGenerator(circleCentresDistribution(generator), circleCentresDistribution(generator), avgRadiusDistribution(generator), 
                     angleIrregularityDistribution(generator), spikeDistribution(generator), this, verbose);
};

void Polygon::Generator2(bool verbose){
    static default_random_engine generator(clock());
    static uniform_int_distribution<int> min_max_interval(-50, 50);
    /*int min = min_max_interval(generator), max = min_max_interval(generator);
    if(min < max){
        space_partition(this, this->numVertices, min, max);
    }
    else{
        space_partition(this, this->numVertices, max, min);
    }*/
    static int min = 0;
    static int max = 10;
    space_partition(this, this->numVertices, min, max);
    min+= min_max_interval(generator);
    max+= min_max_interval(generator);
};

void Polygon::Generator3(bool verbose){}