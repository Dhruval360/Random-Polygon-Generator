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