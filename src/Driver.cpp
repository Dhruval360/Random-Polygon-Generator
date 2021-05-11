#include<stdio.h>
#include<popt.h>
#include<string.h>
#include<pthread.h>
#include<omp.h>
#include<random>
#include<unistd.h>
#include<string>
#include"Polygon.hpp"
using namespace std;

// Default values of parameters
int number_of_polygons = 1, verbose = 0, profiling = 0, graph = 0, dist_analysis = 0;
char *algorithm = NULL, *filename = NULL;

double timer;
long fileSize;

extern float Scale;

Polygon *polygons;
// Distribution for number of vertices
default_random_engine generator(clock());
uniform_int_distribution<unsigned> distribution(10, 500);

int main(int argc, const char** argv){ 
    srand(time(0));
    int choice = 1;//rand()%5 + 1;
    Scale = 1000;
    static struct poptOption options[] = { 
        { "number_of_polygons", 'n',POPT_ARG_INT, &number_of_polygons, 0, "Number of polygons that need to be generated. Default : n=1", "NUM" },
        { "verbose", 'v',POPT_ARG_INT, &verbose, 0, "Set v=1 for verbose output (will slow down the program by some time)", "NUM" },
        { "algorithm", 'a',POPT_ARG_STRING, &algorithm, 0, "Set a=polar or spacePartition or naivePoly to select the algorithm used to generate the polygons", "STR" }, // Name the algorithms
        { "graph", 'g', POPT_ARG_INT, &graph, 0, "Set g=1 to graph the generated polygons", "NUM" },
        { "profiling", 'p', POPT_ARG_INT, &profiling, 0, "Set p=1 for profiling mode", "NUM"},
        { "filename", 'f', POPT_ARG_STRING, &filename, 0, "Enter the filename to which the generated polygons are to be written to in WKT format. Default : map.wkt", "STR"},
        { "distribution", 'd', POPT_ARG_INT, &dist_analysis, 0, "Set d=1 for the analysis of the distribution of the generated polygons onto a single canvas (using OpenGL)", "NUM"},
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
    else if(strcasecmp(algorithm, "polar") && strcasecmp(algorithm, "spacePartition") && strcasecmp(algorithm, "naivePoly")){
        fprintf(stderr, "polygonGenerator: Invalid algorithm name.\npolygonGenerator: Try './bin/polygonGenerator -?' for more information.\n");
        exit(1);
    }
    
    polygons = new Polygon[number_of_polygons];  // Creating an array of polygons
    
    start_timer(total);

    if(verbose){
        if(graph) printf("Graph scale = %f\n", Scale);
        printf("Distribution used = %d\n", choice); // Print distribution name instead
    }

    if(!strcasecmp(algorithm, "polar")){
        #pragma omp parallel for
        for(int i = 0; i < number_of_polygons; i++){
            polygons[i] = Polygon(distribution(generator));
            polygons[i].Generator1(verbose, choice);
        }
    }
    else if(!strcasecmp(algorithm, "spacePartition")){
        #pragma omp parallel for
        for(int i = 0; i < number_of_polygons; i++){
            polygons[i] = Polygon(distribution(generator));
            polygons[i].Generator2(verbose, choice);
        }
    }
    else{
        //#pragma omp parallel for
        for(int i = 0; i < number_of_polygons; i++){
            polygons[i] = Polygon(distribution(generator));
            polygons[i].Generator3(verbose, choice);
        }
    }

    end_timer(total, timer);
    
    // Writing the polygons to the file in WKT format
    if(!profiling){
        printf("Total time taken for generating %u polygons is %lf s\n", number_of_polygons, timer);
        printf("Writing the polygons to the file... \n");
    } 
    
    if(graph){
        pthread_t graphicsThread;
        int ret = pthread_create(&graphicsThread, NULL, GraphicsInit, NULL);
        if(ret) fprintf(stderr, "There was an error launching the graphics thread.\nThe error value returned by pthread_create() is %s\n", strerror(ret));
        writer(polygons, number_of_polygons, filename);  
        if(!profiling) printf("Done\nFile size = %lu B\n", fileSize);
        else printf("%u, %lf, %lu, %s\n", number_of_polygons, timer, fileSize, algorithm);
        pthread_join(graphicsThread, NULL);
    } 
    else{
        writer(polygons, number_of_polygons, filename);
        if(!profiling) printf("Done\nFile size = %lu B\n", fileSize);
        else printf("%u, %lf, %lu, %s\n", number_of_polygons, timer, fileSize, algorithm);
    }    

    if(dist_analysis){
        printf("Plotting the distribution of the generated polygons...\n");
        execlp("python3", "python3", "Distribution.py", (char*) NULL);
    }
    return 0;
}