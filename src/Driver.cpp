#include<stdio.h>
#include<random>
#include<popt.h>
#include <strings.h>
#include"Polygon.hpp"
using namespace std;
//default values of parameters
int number_of_polygons = 1, verbose = false;
char *algorithm = NULL, *filename = NULL;
bool plot = true, metrics = true;

default_random_engine generator(clock());
uniform_int_distribution<unsigned> distribution(10, 500); // Distribution for number of vertices

int main(int argc, const char** argv){ 
    static struct poptOption options[] = { 
        { "number_of_polygons", 'n',POPT_ARG_INT, &number_of_polygons, 0, "Number of polygons that need to be generated. By default, 1 polygon is generated", "NUM" },
        { "verbose", 'v',POPT_ARG_INT, &verbose, 0, "Set v=1 for verbose output", "NUM" },
        { "algorithm", 'a',POPT_ARG_STRING, &algorithm, 0, "Choose the algorithm used to generate the polygons. Available algorithms: polar, spacePartition, chandappa", "STR" }, // Name the algorithms
        { "plot", 'p', POPT_ARG_SHORT, &plot, 0, "Set p=1 to plot the generated polygons", "NUM" },
        { "metrics", 'm', POPT_ARG_SHORT, &metrics, 0, "Set m=1 for timing the program", "NUM"},
        { "filename", 'f', POPT_ARG_STRING, &filename, 0, "enter name", "STR"},
        POPT_AUTOHELP
        { NULL, 0, 0, NULL, 0, NULL, NULL }
    };

    poptContext poptCONT = poptGetContext("main", argc, argv, options, POPT_CONTEXT_KEEP_FIRST);

    //poptSetOtherOptionHelp(poptCONT, "[OPTIONS]* <port>");
    if (argc < 2) {
        poptPrintUsage(poptCONT, stderr, 0);
        exit(1);
    }

    int c; 
    while((c = poptGetNextOpt(poptCONT)) >= 0);
    if (c < -1) { // An error occurred during option processing 
        fprintf(stderr, "polygonGenerator: %s -- \'%s\'\n",
                poptStrerror(c), poptBadOption(poptCONT, POPT_BADOPTION_NOALIAS));
        poptPrintUsage(poptCONT, stderr, 0);
        return 1;
    }

    if(algorithm == NULL){
        fprintf(stderr, "polygonGenerator: Choose an algorithm.\npolygonGenerator: Try './bin/polygonGenerator -?' for more information.\n");
        exit(1);
    } 
    else if(strcasecmp(algorithm, "polar") && strcasecmp(algorithm, "spacePartition") && strcasecmp(algorithm, "chandappa")){
        fprintf(stderr, "polygonGenerator: Invalid algorithm name.\npolygonGenerator: Try './bin/polygonGenerator -?' for more information.\n");
        exit(1);
    }
    // Creating an array of polygons
    Polygon *polygon = new Polygon[number_of_polygons]; 
    
    for(unsigned i = 0; i < number_of_polygons; i++){
        polygon[i] = Polygon(distribution(generator));
        if(!strcasecmp(algorithm, "polar")) polygon[i].Generator1(verbose);
        else if(!strcasecmp(algorithm, "spacePartition")) polygon[i].Generator2(verbose);
        else polygon[i].Generator3(verbose);
    } 
    // Writing the polygons to the file in WKT format
    write(polygon, number_of_polygons, filename);
    return 0;
}