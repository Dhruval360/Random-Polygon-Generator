#include<stdio.h>
#include<math.h>
#include<random>
#include"Polygon.hpp"

#ifndef PI
    #define PI 3.14159265358979323846
#endif

using namespace std;

double clip(double x, double min, double max){
    x = x < min ? min : x;
    x = x > max ? max : x;
    return x;
} 

void polarGenerator(double x, double y, double averageRadius, double irregularity, double spike, Polygon *p, bool verbose){
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

    for(int i = 0; i < p->numVertices; i++){
        angleSteps[i] = lowerBound + uniformDistribution(generator) * (delta);
        sum += angleSteps[i];
    }

    // Normalizing the steps so that the first and last points are the same
    double k = sum / (2*PI);
    for(int i = 0; i < p->numVertices; i++) angleSteps[i] /= k;

    // Generating the points
    double diameter = 2*averageRadius, radial_distance;
    double angle = uniformDistribution(generator) * 2*PI;
    for(int i = 0; i < p->numVertices; i++){
        radial_distance = clip(normalDistribution(generator), 0, diameter);
        p->coordinates.push_back(pair<double, double>(x + radial_distance*cos(angle), y + radial_distance*sin(angle)));
        angle += angleSteps[i];
    }
    if(verbose){
        printf("Number of vertices = %3u ", p->numVertices);
        printf("Center = (%11.6lf, %11.6lf), averageRadius = %.6lf, irregularity = %8.6lf, spike = %.6lf\n", x, y, averageRadius, irregularity, spike);
    }
}