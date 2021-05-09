#include<random>
#include<iostream>
#include"Polygon.hpp"

using namespace std;

extern double timer; // Used for storing the calculated time taken
extern float Scale;
extern unsigned number_of_polygons;
extern short graph;

static int GraphingScaleSetter(unsigned scale){
    Scale = scale;
    if(graph) printf("Graph scale = %f\n", Scale);
    return Scale;
}

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
    static int _ = GraphingScaleSetter(1000);
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
    static int scale = GraphingScaleSetter(10);
    static default_random_engine generator(clock());
    static uniform_int_distribution<int> uniform(-500, 500);
    static binomial_distribution<int> binomial(9,0.5);
    static geometric_distribution<int> geometric(0.1);
    static poisson_distribution<int> poisson(3);
    static normal_distribution<double> normal(0,1);
    static discrete_distribution<int> discrete(5,10);
    static int min, max;
    start_timer(start);
    switch(choice){
    case 1:
    static int a = GraphingScaleSetter(500);
    min = uniform(generator); max = uniform(generator);
    if(min > max){
        int temp = min;
        min = max;
        max = temp;
    }
    space_partition(this, min, max, verbose);
    if(verbose){
        cout<< "Uniform Distribution" << endl;
    }
    break;
    case 2:
    static int b = GraphingScaleSetter(10);
    min = binomial(generator); max = binomial(generator);
    if(min > max){
        int temp = min;
        min = max;
        max = temp;
    }
    space_partition(this, min, max, verbose);
    if(verbose){
        cout<< "Binomial Distribution" << endl;
    }
    break;
    case 3:
    static int c = GraphingScaleSetter(70);
    min = geometric(generator) - geometric(generator)*0.5; max = geometric(generator) - geometric(generator)*0.5;
    if(min > max){
        int temp = min;
        min = max;
        max = temp;
    }
    space_partition(this, min, max, verbose);
    if(verbose){
        cout<< "Geometric Distribution" << endl;
    }
    break;
    case 4:
    static int d = GraphingScaleSetter(10);
    min = poisson(generator); max = poisson(generator);
    if(min > max){
        int temp = min;
        min = max;
        max = temp;
    }
    space_partition(this, min, max, verbose);
    if(verbose){
        cout<< "Poisson Distribution" << endl;
    }
    break;
    case 5:
    static int e = GraphingScaleSetter(1);
    min = normal(generator); max = normal(generator);
    if(min > max){
        int temp = min;
        min = max;
        max = temp;
    }
    space_partition(this, min, max, verbose);
    if(verbose){
        cout<< "Normal Distribution" << endl;
    }
    break;
    case 6:
    static int f = GraphingScaleSetter(4);
    min = discrete(generator); max = discrete(generator);
    if(min > max){
        int temp = min;
        min = max;
        max = temp;
    }
    space_partition(this, min, max, verbose);
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