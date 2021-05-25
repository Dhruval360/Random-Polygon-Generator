#include<stdio.h>
#include<math.h>
#include<random>
#include"Polygon.hpp"
#define PI 3.14159265358979323846

using namespace std;

extern double timer;

void polarGenerator(double x, double y, double averageRadius, double irregularity, double spike, Polygon *p, bool verbose){
    start_timer(start);
    double angleSteps[p->numVertices];
    static default_random_engine generator(clock());
    static uniform_real_distribution<double> uniformDistribution(0.0, 1.0);

    normal_distribution<double> normalDistribution(averageRadius, spike);

    irregularity = irregularity * 2 * PI / p->numVertices;
    spike = clip(spike, 0, 1) * averageRadius;
    double lowerBound = (2 * PI / p->numVertices) - irregularity;
    double upperBound = (2 * PI / p->numVertices) + irregularity;
    double delta = upperBound - lowerBound;
    double sum = 0;

    for(unsigned i = 0; i < p->numVertices; i++){
        angleSteps[i] = lowerBound + uniformDistribution(generator) * (delta);
        sum += angleSteps[i];
    }

    // Normalizing the steps so that the first and last points are the same
    double k = sum / (2*PI);

    for(unsigned i = 0; i < p->numVertices; i++) angleSteps[i] /= k;

    // Generating the points
    double diameter = 2*averageRadius, radial_distance;
    double angle = uniformDistribution(generator) * 2*PI;
    for(unsigned i = 0; i < p->numVertices; i++){
        radial_distance = clip(normalDistribution(generator), 0, diameter);
        p->coordinates.push_back(pair<double, double>(x + radial_distance*cos(angle), y + radial_distance*sin(angle)));
        angle += angleSteps[i];
    }
    end_timer(start, timer);
    if(verbose) printf("Number of vertices = %3u, Center = (%10.5lf, %10.5lf), averageRadius = %10.5lf, irregularity = %8.6lf, spike = %10.5lf | Time taken for generation = %lf s\n",  p->numVertices, x, y, averageRadius, irregularity, spike, timer);
}