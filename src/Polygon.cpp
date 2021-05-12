#include<random>
#include<stdio.h>
#include"Polygon.hpp"

using namespace std;

extern double timer; // Used for storing the calculated time taken
extern float Scale;
extern unsigned number_of_polygons;
extern short graph;

static double clip(double x, double min, double max){
    x = x < min ? min : x;
    x = x > max ? max : x;
    return x;
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

void Polygon::Generator1(bool verbose, int choice){
    static default_random_engine generator(clock());
    static uniform_real_distribution<double> uniform(-Scale/2, Scale/2);
    static binomial_distribution<int> binomial(Scale/2, 0.5);
    static geometric_distribution<int> geometric(Scale/10000); // Needs to be redone
    static poisson_distribution<int> poisson(Scale/10);
    static normal_distribution<double> normal(3*Scale/500, 2);
    static uniform_real_distribution<double> avgRadiusDistribution(Scale/20, Scale/2);
    static uniform_real_distribution<double> angleIrregularityDistribution(0.0, 1.0);
    static uniform_real_distribution<double> spikeDistribution(0.0, Scale/20);
    double x,y;
    switch(choice){
        case 1:
            x = uniform(generator); 
            y = uniform(generator);
            polarGenerator(x , y, avgRadiusDistribution(generator), 
                     angleIrregularityDistribution(generator), spikeDistribution(generator), this, verbose);
            break;
        case 2:
            x = binomial(generator) - binomial(generator); 
            y = binomial(generator) - binomial(generator);
            polarGenerator(x , y, avgRadiusDistribution(generator), 
                     angleIrregularityDistribution(generator), spikeDistribution(generator), this, verbose);
            break;
        case 3:
            x = geometric(generator) - geometric(generator)*0.5; 
            y = geometric(generator) - geometric(generator)*0.5;
            polarGenerator(clip(x, -Scale/2, Scale/2) , clip(y, -Scale/2, Scale/2), avgRadiusDistribution(generator), 
                     angleIrregularityDistribution(generator), spikeDistribution(generator), this, verbose);
            break;
        case 4:
            x = poisson(generator); 
            y = poisson(generator);
            polarGenerator(x , y, avgRadiusDistribution(generator), 
                     angleIrregularityDistribution(generator), spikeDistribution(generator), this, verbose);
            break;
        case 5:
            x = normal(generator); 
            y = normal(generator);
            polarGenerator(x , y, avgRadiusDistribution(generator), 
                     angleIrregularityDistribution(generator), spikeDistribution(generator), this, verbose);
            break;
        default:
            break;
    }
};

void Polygon::Generator2(bool verbose, int choice){
    static int min, max;
    //Scale = 1;
    static default_random_engine generator(clock());
    static uniform_int_distribution<int> uniform(-Scale, Scale);
    static binomial_distribution<int> binomial(Scale, 0.5);
    static geometric_distribution<int> geometric(Scale/10000);
    static poisson_distribution<int> poisson(Scale/10);
    static normal_distribution<double> normal(0, Scale/2);
    switch(choice){
        case 1:
            min = uniform(generator); 
            max = uniform(generator);
            if(min > max){
                int temp = min;
                min = max;
                max = temp;
            }
            space_partition(this, min, max, verbose);
            break;
        case 2:
            min = binomial(generator) - binomial(generator); 
            max = binomial(generator) - binomial(generator);
            if(min > max){
                int temp = min;
                min = max;
                max = temp;
            }
            space_partition(this, clip(15*min, -Scale, Scale), clip(15*max, -Scale, Scale), verbose);
            break;
        case 3:
            min = geometric(generator) - geometric(generator)*0.5; 
            max = geometric(generator) - geometric(generator)*0.5;
            if(min > max){
                int temp = min;
                min = max;
                max = temp;
            }
            space_partition(this, clip(55*min, -Scale, Scale), clip(55*max, -Scale, Scale), verbose);
            break;
        case 4:
            min = poisson(generator) - poisson(generator); 
            max = poisson(generator) - poisson(generator); 
            if(min > max){
                int temp = min;
                min = max;
                max = temp;
            }
            space_partition(this, clip(50*min, -Scale, Scale), clip(50*max, -Scale, Scale), verbose);
            break;
        case 5:
            min = normal(generator); max = normal(generator);
            if(min > max){
                int temp = min;
                min = max;
                max = temp;
            }
            space_partition(this, clip(min, -Scale, Scale), clip(max, -Scale, Scale), verbose);
            break;
        default:
            break;
    }
};

void Polygon::Generator3(bool verbose, int choice){
    naivePolygon(this, verbose);
}