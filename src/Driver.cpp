#include<stdio.h>
#include<random>
#include<popt.h>
#include<strings.h>
#include"Polygon.hpp"

using namespace std;

// Default values of parameters
int number_of_polygons = 1, verbose = false;
char *algorithm = NULL, *filename = NULL;
bool graph = true, profiling = false;

double timer;

Polygon *polygons;

default_random_engine generator(clock());
uniform_int_distribution<unsigned> distribution(10, 500); // Distribution for number of vertices

int main(int argc, const char** argv){ 
    static struct poptOption options[] = { 
        { "number_of_polygons", 'n',POPT_ARG_INT, &number_of_polygons, 0, "Number of polygons that need to be generated. By default, 1 polygons is generated", "NUM" },
        { "verbose", 'v',POPT_ARG_INT, &verbose, 0, "Set v=1 for verbose output", "NUM" },
        { "algorithm", 'a',POPT_ARG_STRING, &algorithm, 0, "Choose the algorithm used to generate the polygons.\n\t\t\t\t   Available algorithms:\n\t\t\t\t\t* polar\n\t\t\t\t\t* spacePartition\n\t\t\t\t\t* chandappa", "STR" }, // Name the algorithms
        { "graph", 'g', POPT_ARG_SHORT, &graph, 0, "Set g=1 to graph the generated polygons", "NUM" },
        { "profiling", 'p', POPT_ARG_SHORT, &profiling, 0, "Set p=1 for timing the program", "NUM"},
        { "filename", 'f', POPT_ARG_STRING, &filename, 0, "Enter the filename to which the polygons is to be written to. Default : map.wkt", "STR"},
        POPT_AUTOHELP
        { NULL, 0, 0, NULL, 0, NULL, NULL }
    };

    poptContext poptCONT = poptGetContext("main", argc, argv, options, POPT_CONTEXT_KEEP_FIRST);

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
    polygons = new Polygon[number_of_polygons]; 
    
    start_timer(total);

    for(int i = 0; i < number_of_polygons; i++){
        polygons[i] = Polygon(distribution(generator));
        if(!strcasecmp(algorithm, "polar")) polygons[i].Generator1(verbose);
        else if(!strcasecmp(algorithm, "spacePartition")) polygons[i].Generator2(verbose);
        else polygons[i].Generator3(verbose);
        if(profiling) printf("| Time taken for generation = %lf s\n", timer);
        else if(verbose) printf("\n");
    } 

    end_timer(total, timer);
    printf("Total time taken for generating %u polygons is %lf s\n", number_of_polygons, timer);
    
    // Writing the polygons to the file in WKT format
    printf("Writing the polygons to the file... \n");
    write(polygons, number_of_polygons, filename);    
    if(graph) GraphicsInit();
    return 0;
}