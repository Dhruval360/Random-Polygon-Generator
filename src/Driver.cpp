#include<random>
#include<stdio.h>
#include<popt.h>
#include<time.h>
#include"Polygon.hpp"

using namespace std;

int number_of_polygons, verbose;
char *algorithm;
bool plot = true, metrics = true;

default_random_engine generator(clock());
uniform_int_distribution<unsigned> distribution(10, 500); // Distribution for number of vertices
uniform_real_distribution<double> distribution2(-50, 50); // Distributi
uniform_real_distribution<double> distribution3(50, 500);
uniform_real_distribution<double> distribution4(0.0, 1.0);

int main(int argc, const char** argv){ 
    static struct poptOption options[] = { 
        { "number_of_polygons", 'n',POPT_ARG_INT, &number_of_polygons, 0, "Number of polygons that need to be generated", "NUM" },
        { "verbose", 'v',POPT_ARG_INT, &verbose, 0, "Set v=1 for verbose output", "NUM" },
        { "algorithm", 'a',POPT_ARG_STRING, &algorithm, 0, "Choose the algorithm used to generate the polygons", "STR" }, // Name the algorithms
        { "plot", 'p', POPT_ARG_SHORT, &plot, 0, "Set p=1 to plot the generated polygons", "NUM" },
        { "metrics", 'm', POPT_ARG_SHORT, &metrics, 0, "Set m=1 for timing the program", "NUM"},
        POPT_AUTOHELP
        { NULL, 0, 0, NULL, 0, NULL, NULL }
    };

    poptContext poptCONT = poptGetContext("main", argc, argv, options, POPT_CONTEXT_KEEP_FIRST);
    int c = 0; while((c = poptGetNextOpt(poptCONT)) >= 0);

    Polygon *p = new Polygon[number_of_polygons];
    for(unsigned i = 0; i < number_of_polygons; i++){
        p[i] = Polygon(distribution(generator));
        p[i].Generator1(distribution2(generator), distribution2(generator), distribution3(generator), distribution4(generator), distribution4(generator));
    } 
    write(p, number_of_polygons);
    return 0;
}