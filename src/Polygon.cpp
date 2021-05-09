#include<random>
#include<iostream>
#include"Polygon.hpp"

using namespace std;

extern double timer; // Used for storing the calculated time taken
extern float Scale;
extern unsigned number_of_polygons;

Polygon::Polygon(unsigned int numVertices){
    static default_random_engine generator(clock());
    static uniform_real_distribution<float> ColorDistribution(0.25, 0.8);
    this->numVertices = numVertices;
    this->coordinates.reserve(numVertices);
    this->color[0] = ColorDistribution(generator);
    this->color[1] = ColorDistribution(generator);
    this->color[2] = ColorDistribution(generator);
}

void Polygon::Generator1(bool verbose){
    static int _ = printf("Graph scale = %f\n", Scale = 1000);
    static default_random_engine generator(clock());
    static uniform_real_distribution<double> circleCentresDistribution(-500, 500); 
    static uniform_real_distribution<double> avgRadiusDistribution(50, 500);
    static uniform_real_distribution<double> angleIrregularityDistribution(0.0, 1.0);
    static uniform_real_distribution<double> spikeDistribution(0.0, 55.0);
    start_timer(start);
    polarGenerator(circleCentresDistribution(generator), circleCentresDistribution(generator), avgRadiusDistribution(generator), 
                     angleIrregularityDistribution(generator), spikeDistribution(generator), this, verbose);
    end_timer(start, timer);
};

void Polygon::Generator2(bool verbose, int choice){
    static int _ = printf("Graph scale = %f\n", Scale = number_of_polygons);
    static default_random_engine generator(clock());
    static uniform_int_distribution<int> uniform(-1000, 1000);
    static bernoulli_distribution bernoulli(3);
    static binomial_distribution<int> binomial(9,0.5);
    static geometric_distribution<int> geometric(0.3);
    static poisson_distribution<int> poisson(3);
    static normal_distribution<double> normal(3,2);
    static discrete_distribution<int> discrete(3,2);
    static int min, max;
    start_timer(start);
    switch(choice){
    case 1:
    min = uniform(generator); max = uniform(generator);space_partition(this, min, max, verbose);
    if(verbose){
        cout<< "Uniform Distribution" << endl;
    }
    break;
    case 2:
    min = bernoulli(generator); max = bernoulli(generator);space_partition(this, min, max, verbose);
    if(verbose){
        cout<< "Bernoulli Distribution" << endl;
    }
    break;
    case 3:
    min = binomial(generator); max = binomial(generator);space_partition(this, min, max, verbose);
    if(verbose){
        cout<< "Binomial Distribution" << endl;
    }
    break;
    case 4:
    min = geometric(generator); max = geometric(generator);space_partition(this, min, max, verbose);
    if(verbose){
        cout<< "Geometric Distribution" << endl;
    }
    break;
    case 5:
    min = poisson(generator); max = poisson(generator);space_partition(this, min, max, verbose);
    if(verbose){
        cout<< "Poisson Distribution" << endl;
    }
    break;
    case 6:
    min = normal(generator); max = normal(generator);space_partition(this, min, max, verbose);
    if(verbose){
        cout<< "Normal Distribution" << endl;
    }
    break;
    case 7:
    min = discrete(generator); max = discrete(generator);space_partition(this, min, max, verbose);
    if(verbose){
        cout<< "Discrete Distribution" << endl;
    }
    break;
    default:
        break;
    }
    end_timer(start, timer);
    /*static int min = 0;
    static int max = 100;
    min+= min_max_interval(generator);
    max+= min_max_interval(generator);*/
};

void Polygon::Generator3(bool verbose){}