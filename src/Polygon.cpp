#include<stdio.h>
#include<random>
#include<set>
#include<utility>
#include"Polygon.hpp"

using namespace std;

extern double timer; // Used for storing the calculated time taken
extern float Scale;
extern unsigned number_of_polygons;
extern short graph;

double clip(double x, double min = -Scale, double max = Scale){
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
    static geometric_distribution<int> geometric(Scale/10000); 
    static poisson_distribution<int> poisson(Scale/10);
    static normal_distribution<double> normal(0, Scale/3);
    static uniform_real_distribution<double> avgRadiusDistribution(Scale/20, Scale/2);
    static uniform_real_distribution<double> angleIrregularityDistribution(0.0, 1.0);
    static uniform_real_distribution<double> spikeDistribution(0.0, Scale/20);
    double x, y;
    switch(choice){
        case 1:
            x = uniform(generator); 
            y = uniform(generator);
            polarGenerator(x, y, avgRadiusDistribution(generator), 
                     angleIrregularityDistribution(generator), spikeDistribution(generator), this, verbose);
            break;
        case 2:
            x = (binomial(generator) - binomial(generator)) * binomial(generator) * 0.05; 
            y = (binomial(generator) - binomial(generator)) * binomial(generator) * 0.05;
            polarGenerator(clip(x, -Scale/2, Scale/2) , clip(y, -Scale/2, Scale/2), avgRadiusDistribution(generator), 
                     angleIrregularityDistribution(generator), spikeDistribution(generator), this, verbose);
            break;
        case 3:
            x = geometric(generator) - geometric(generator)*0.5; 
            y = geometric(generator) - geometric(generator)*0.5;
            polarGenerator(clip(10*x, -Scale/2, Scale/2) , clip(10*y, -Scale/2, Scale/2), avgRadiusDistribution(generator), 
                     angleIrregularityDistribution(generator), spikeDistribution(generator), this, verbose);
            break;
        case 4:
            x = poisson(generator); 
            y = poisson(generator);
            polarGenerator(clip(x, -Scale/2, Scale/2) , clip(y, -Scale/2, Scale/2), avgRadiusDistribution(generator), 
                     angleIrregularityDistribution(generator), spikeDistribution(generator), this, verbose);
            break;
        case 5:
            x = normal(generator); 
            y = normal(generator);
            polarGenerator(clip(x, -Scale/2, Scale/2) , clip(y, -Scale/2, Scale/2), avgRadiusDistribution(generator), 
                     angleIrregularityDistribution(generator), spikeDistribution(generator), this, verbose);
            break;
        default:
            break;
    }
};

void Polygon::Generator2(bool verbose, int choice){
    set<pair<double, double>> temp; // A set to store non duplicate points only
    static default_random_engine generator(clock());
    static uniform_real_distribution<double> random_ratio(0, 1);
    static uniform_int_distribution<int> uniform(-Scale, Scale);
    static binomial_distribution<int> binomial(Scale, 0.5);
    static geometric_distribution<int> geometric(Scale/10000);
    static poisson_distribution<int> poisson(Scale/10);
    static normal_distribution<double> normal(0, Scale/2);
    double min, max;
    switch(choice){
        case 1:
            min = uniform(generator); 
            max = uniform(generator);
            if(min > max){
                double temp = min;
                min = max;
                max = temp;
            }
            while(temp.size() < this->numVertices){
                temp.insert(make_pair(min + (max - min) * random_ratio(generator), (max - min) * random_ratio(generator)));
            }
            for(auto pt : temp)	this->coordinates.push_back(pt);
            spacePartition(this, min, max, verbose);
            break;
        case 2:
            min = clip(15*(binomial(generator) - binomial(generator))); 
            max = clip(15*(binomial(generator) - binomial(generator)));
            if(min > max){
                double temp = min;
                min = max;
                max = temp;
            }
            while(temp.size() < this->numVertices){
                temp.insert(make_pair(min + (max - min) * random_ratio(generator), (max - min) * random_ratio(generator)));
            }
            for(auto pt : temp)	this->coordinates.push_back(pt);
            spacePartition(this, min, max, verbose);
            break;
        case 3:
            min = clip(55*(geometric(generator) - geometric(generator)*0.5)); 
            max = clip(55*(geometric(generator) - geometric(generator)*0.5));
            if(min > max){
                double temp = min;
                min = max;
                max = temp;
            }
            while(temp.size() < this->numVertices){
                temp.insert(make_pair(min + (max - min) * random_ratio(generator), (max - min) * random_ratio(generator)));
            }
            for(auto pt : temp)	this->coordinates.push_back(pt);
            spacePartition(this, min, max, verbose);
            break;
        case 4:
            min = clip(50*(poisson(generator) - poisson(generator))); 
            max = clip(50*(poisson(generator) - poisson(generator))); 
            if(min > max){
                double temp = min;
                min = max;
                max = temp;
            }
            while(temp.size() < this->numVertices){
                temp.insert(make_pair(min + (max - min) * random_ratio(generator), (max - min) * random_ratio(generator)));
            }
            for(auto pt : temp)	this->coordinates.push_back(pt);
            spacePartition(this, min, max, verbose);
            break;
        case 5:
            min = clip(normal(generator)); 
            max = clip(normal(generator));
            if(min > max){
                double temp = min;
                min = max;
                max = temp;
            }
            while(temp.size() < this->numVertices){
                temp.insert(make_pair(min + (max - min) * random_ratio(generator), (max - min) * random_ratio(generator)));
            }
            for(auto pt : temp)	this->coordinates.push_back(pt);
            spacePartition(this, min, max, verbose);
            break;
        default:
            break;
    }
};

void Polygon::Generator3(bool verbose, int choice){
    set<pair<double, double>> temp; // A set to store non duplicate points only
    static default_random_engine generator(clock());
    static uniform_int_distribution<int> uniform(-Scale, Scale);
    static binomial_distribution<int> binomial(Scale, 0.5);
    static geometric_distribution<int> geometric(Scale/10000); // Broken
    static poisson_distribution<int> poisson(Scale/10);        // Broken
    static normal_distribution<double> normal(0, Scale/2);
    double x, y;
    switch(choice){
        case 1:
            while(temp.size() < this->numVertices){
                x = uniform(generator); 
                y = uniform(generator);
                temp.insert(make_pair(x, y));
            }
            for(auto pt : temp)	this->coordinates.push_back(pt);
            break;
        case 2:
            while(temp.size() < this->numVertices){
                x = clip(15*(binomial(generator) - binomial(generator))); 
                y = clip(15*(binomial(generator) - binomial(generator)));
                temp.insert(make_pair(x, y));
            }
            for(auto pt : temp)	this->coordinates.push_back(pt);
            break;
        case 3:
            while(temp.size() < this->numVertices){
                x = clip(55*(geometric(generator) - geometric(generator)*0.5)); 
                y = clip(55*(geometric(generator) - geometric(generator)*0.5));
                temp.insert(make_pair(x, y));
            }
            for(auto pt : temp)	this->coordinates.push_back(pt);
            break;
        case 4:
            while(temp.size() < this->numVertices){
                x = clip(50*(poisson(generator) - poisson(generator))); 
                y = clip(50*(poisson(generator) - poisson(generator))); 
                temp.insert(make_pair(x, y));
            }
            for(auto pt : temp)	this->coordinates.push_back(pt);
            break;
        case 5:
            while(temp.size() < this->numVertices){
                x = clip(normal(generator)); 
                y = clip(normal(generator));
                temp.insert(make_pair(x, y));
            }
            for(auto pt : temp)	this->coordinates.push_back(pt);
            break;
        default:
            break;
    }
    naivePolygon(this, verbose);
}