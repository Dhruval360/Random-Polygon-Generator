#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<random>
#include"polygon.hpp"
#ifndef PI
    #define PI 3.14159265358979323846
#endif
using namespace std;

default_random_engine generator(10);
uniform_real_distribution<double> distribution(0.0, 1.0);

double clip(double x, double min, double max){
    x = x < min ? min : x;
    x = x > max ? max : x;
    return x;
} 

void polygonGenerator(double x, double y, double averageRadius, double irregularity, double spike, Polygon &p){
    normal_distribution<double> normal(averageRadius, spike);

    irregularity = clip(irregularity, 0, 1) * 2 * PI / p.numVertices;
    spike = clip(spike, 0, 1) * averageRadius;
    double lowerBound = (2 * PI / p.numVertices) - irregularity;
    double upperBound = (2 * PI / p.numVertices) + irregularity;
    double delta = upperBound - lowerBound;
    double sum = 0;
    double *angleSteps = (double*)malloc(sizeof(double) * p.numVertices);
    for(int i = 0; i < p.numVertices; i++){
        angleSteps[i] = lowerBound + distribution(generator) * (delta);
        sum += angleSteps[i];
    }

    // Normalizing the steps so that the first and last points are the same
    double k = sum / (2*PI);
    for(int i = 0; i < p.numVertices; i++) angleSteps[i] /= k;

    // Generating the points
    double diameter = 2*averageRadius;
    double angle = distribution(generator) * 2*PI;
    for(int i = 0; i < p.numVertices; i++){
        double temp = clip(normal(generator), 0, diameter);
        p.coordinates[i].first  = x + temp*cos(angle);
        p.coordinates[i].second = y + temp*sin(angle);
        angle += angleSteps[i];
    }
    free(angleSteps);
}