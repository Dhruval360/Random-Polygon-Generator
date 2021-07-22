#include <stdio.h>
#include <set>
#include <random>
#include <vector>
#include <utility>
#include <iostream>
#include "Classes.hpp"
#include "../Utils/Geom.hpp"

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

/************************ OverLoaded Operator Functions ************************/

ostream& operator << (ostream& out, pair<double,double>& rhs){
	out << "(" << rhs.first << "," << rhs.second << ")";
	return out;
}

pair<double,double> operator - (pair<double,double>& p1, pair<double,double>& p2){
	return {p1.first-p2.first, p1.second-p2.second};
}

bool operator == (Edge& lhs, Edge& rhs){
    return rhs.startVertex == lhs.startVertex && rhs.endVertex == lhs.endVertex;
}

ostream& operator << (ostream& out, Edge& rhs){
		out << rhs.startVertex << "->" << rhs.endVertex;
		return out;
}
/*******************************************************************************/


/************************* Edge Class Member Functions *************************/

Edge::Edge(pair<double,double> p1, pair<double,double> p2){
	this->startVertex = p1;
	this->endVertex = p2;
}

int Edge::indexIn(vector <Edge>& arr){
	for(int i = 0; i < (int)arr.size(); i++)
		if(arr.at(i) == *this) return i;
    return -1;
}

bool Edge::intersectingUtil(pair<double,double> &a, pair<double,double> &b){
		// Alias for start and end vertex
		auto& c = this->startVertex;
		auto& d = this->endVertex;
        return (isccw(a, c, d) != isccw(b, c, d) && isccw(a, b, d) != isccw(a, b, c));
}


bool Edge::isIntersecting(vector <Edge> &edges, bool log){
    // Check for all the edges of the polygon with the edge e for intersection
    for(Edge& iterE : edges){
        /*
        If edge to be checked and iterEdge share same end points,
        then they can lie on each other at max but cannot intersect.
        Hence checking for that case.
        */
        if(iterE.startVertex == this->startVertex || 
           iterE.startVertex == this->endVertex   ||
             iterE.endVertex == this->startVertex ||
             iterE.endVertex == this->endVertex) continue;
        
        // Check for orientation of edge points
		if(this->intersectingUtil(iterE.startVertex,iterE.endVertex)){
			if(log){
				Edge temp(iterE.startVertex, iterE.endVertex);
				cout << "The intersecting edges are : " << temp;
			}
			return true;
		}
    }
    return false;
}


bool Edge::isValidEdge(vector <Edge>& edges, pair<double,double>& p){
	// Create an edge from e.start to the point
	Edge e1(this->startVertex,p);

	// Create an edge from p to e.end
	Edge e2(p,this->endVertex);

	// Check if the edges are intersecting with the polygon
	if(!e1.isIntersecting(edges) && !e2.isIntersecting(edges)) return true;
	return false;
}

/*******************************************************************************/


/*********************** Polygon Class Member Functions ************************/

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
            polarGenerator(x, y, avgRadiusDistribution(generator), angleIrregularityDistribution(generator),
                           spikeDistribution(generator), this, verbose);
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
    set<pair<double,double>> temp; // A set to store non duplicate points only
    static default_random_engine generator(clock());
    static uniform_real_distribution<double> random_ratio(0, 1);
    static uniform_int_distribution<int> uniform(-Scale, Scale);
    static binomial_distribution<int> binomial(Scale, 0.5);
    static geometric_distribution<int> geometric(Scale/10000);
    static poisson_distribution<int> poisson(Scale/10);
    static normal_distribution<double> normal(0, Scale/2);
    double x, y;
    switch(choice){
        case 1:
            while(temp.size() < this->numVertices){
                x = uniform(generator); 
                y = uniform(generator);
                temp.insert({x, y});
            }
            for(auto&pt : temp)	this->coordinates.push_back(pt);
            break;
        case 2:
            while(temp.size() < this->numVertices){
                x = clip(15*(binomial(generator) - binomial(generator))); 
                y = clip(15*(binomial(generator) - binomial(generator)));
                temp.insert({x, y});
            }
            for(auto&pt : temp)	this->coordinates.push_back(pt);
            break;
        case 3:
            while(temp.size() < this->numVertices){
                x = clip(55*(geometric(generator) - geometric(generator)*0.5)); 
                y = clip(55*(geometric(generator) - geometric(generator)*0.5));
                temp.insert({x, y});
            }
            for(auto&pt : temp)	this->coordinates.push_back(pt);
            break;
        case 4: 
            while(temp.size() < this->numVertices){
                x = clip(50*(poisson(generator) - poisson(generator))); 
                y = clip(50*(poisson(generator) - poisson(generator)));
                temp.insert({x, y});
            }
            for(auto&pt : temp)	this->coordinates.push_back(pt);
            break;
        case 5:
            while(temp.size() < this->numVertices){
                x = clip(normal(generator)); 
                y = clip(normal(generator));
                temp.insert({x, y});
            }
            for(auto&pt : temp)	this->coordinates.push_back(pt);
            break;
        default:
            break;
    }
    spacePartition(this, verbose);
};

void Polygon::Generator3(bool verbose, int choice){
    set<pair<double,double>> temp; // A set to store non duplicate points only
    static default_random_engine generator(clock());
    static uniform_int_distribution<int> uniform(-Scale, Scale);
    static binomial_distribution<int> binomial(Scale, 0.5);
    static geometric_distribution<int> geometric(Scale/10000); 
    static poisson_distribution<int> poisson(Scale/10);        
    static normal_distribution<double> normal(0, Scale/2);
    double x, y;
    switch(choice){
        case 1:
            while(temp.size() < this->numVertices){
                x = uniform(generator); 
                y = uniform(generator);
                temp.insert({x, y});
            }
            for(auto&pt : temp)	this->coordinates.push_back(pt);
            break;
        case 2:
            while(temp.size() < this->numVertices){
                x = clip(15*(binomial(generator) - binomial(generator))); 
                y = clip(15*(binomial(generator) - binomial(generator)));
                temp.insert({x, y});
            }
            for(auto&pt : temp)	this->coordinates.push_back(pt);
            break;
        case 3:
            while(temp.size() < this->numVertices){
                x = clip(55*(geometric(generator) - geometric(generator)*0.5)); 
                y = clip(55*(geometric(generator) - geometric(generator)*0.5));
                temp.insert({x, y});
            }
            for(auto&pt : temp)	this->coordinates.push_back(pt);
            break;
        case 4:
            while(temp.size() < this->numVertices){
                x = clip(50*(poisson(generator) - poisson(generator))); 
                y = clip(50*(poisson(generator) - poisson(generator))); 
                temp.insert({x, y});
            }
            for(auto&pt : temp)	this->coordinates.push_back(pt);
            break;
        case 5:
            while(temp.size() < this->numVertices){
                x = clip(normal(generator)); 
                y = clip(normal(generator));
                temp.insert({x, y});
            }
            for(auto&pt : temp)	this->coordinates.push_back(pt);
            break;
        default:
            break;
    }
    naivePolygon(this, verbose);
}

void Polygon::constructEdges(){
	unsigned n = this->coordinates.size();
	for (unsigned i = 0; i < n-1; ++i){ 
		Edge e1(this->coordinates[i], this->coordinates[i+1]);
		this->edges.push_back(e1);
	}
	// Connect the starting and the last vertex
	Edge e2(this->coordinates[n-1], this->coordinates[0]);
	this->edges.push_back(e2);
}

bool Polygon::validityCheck(){
    // Construct the edges of the Polygon
    this->constructEdges();

    // For each edge, check for intersection
    for(auto& edge : this->edges){
        if(edge.isIntersecting(this->edges,true)){
            cout << " and "<< edge;
            return false;
        }
    }
    // If not returned till now, all edges are valid
    return true;
}

/*******************************************************************************/