#include<random>
#include<stdio.h>
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

void Polygon::Generator1(bool verbose, int choice){
    static default_random_engine generator(clock());
    static uniform_int_distribution<int> uniform(-Scale/2, Scale/2);
    static binomial_distribution<int> binomial(9,0.5);
    static geometric_distribution<int> geometric(0.1);
    static poisson_distribution<int> poisson(3);
    static normal_distribution<double> normal(0,1);
    static uniform_real_distribution<double> avgRadiusDistribution(Scale/20, Scale/2);
    static uniform_real_distribution<double> angleIrregularityDistribution(0.0, 1.0);
    static uniform_real_distribution<double> spikeDistribution(0.0, 55.0);
    double x,y;
    switch(choice){
        case 1:
            //static int a = GraphingScaleSetter(500);
            x = uniform(generator); y = uniform(generator);
            polarGenerator(x , y, avgRadiusDistribution(generator), 
                     angleIrregularityDistribution(generator), spikeDistribution(generator), this, verbose);
            if(verbose)printf("Uniform Distribution, ");
            break;
        case 2:
            //static int b = GraphingScaleSetter(10);
            x = binomial(generator); 
            polarGenerator(x , y, avgRadiusDistribution(generator), 
                     angleIrregularityDistribution(generator), spikeDistribution(generator), this, verbose);
            if(verbose)printf("Binomial Distribution, ");
            break;
        case 3:
            //static int c = GraphingScaleSetter(70);
            x = geometric(generator) - geometric(generator)*0.5; y = geometric(generator) - geometric(generator)*0.5;
            polarGenerator(x , y, avgRadiusDistribution(generator), 
                     angleIrregularityDistribution(generator), spikeDistribution(generator), this, verbose);
            if(verbose)printf("Geometric Distribution, ");
            break;
        case 4:
            //static int d = GraphingScaleSetter(10);
            x = poisson(generator); y= poisson(generator);
            polarGenerator(x , y, avgRadiusDistribution(generator), 
                     angleIrregularityDistribution(generator), spikeDistribution(generator), this, verbose);
            if(verbose)printf("Poisson Distribution, ");
            break;
        case 5:
            //static int e = GraphingScaleSetter(1);
            x = normal(generator); y= normal(generator);
            polarGenerator(x , y, avgRadiusDistribution(generator), 
                     angleIrregularityDistribution(generator), spikeDistribution(generator), this, verbose);
            if(verbose)printf("Normal Distribution, ");
            break;
        default:
            break;
    }
};

void Polygon::Generator2(bool verbose, int choice){
    static int min, max;
    static default_random_engine generator(clock());
    static uniform_int_distribution<int> uniform(-500, 500);
    static binomial_distribution<int> binomial(9,0.5);
    static geometric_distribution<int> geometric(0.1);
    static poisson_distribution<int> poisson(3);
    static normal_distribution<double> normal(0,1);
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
            if(verbose)printf("Uniform Distribution, ");
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
            if(verbose)printf("Binomial Distribution, ");
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
            if(verbose)printf("Geometric Distribution, ");
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
            if(verbose)printf("Poisson Distribution, ");
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
            if(verbose)printf("Normal Distribution, ");
            break;
        default:
            break;
    }
};

void Polygon::Generator3(bool verbose, int choice){
    static int _ = GraphingScaleSetter(1000);
    naivePolygon(this, verbose);
}