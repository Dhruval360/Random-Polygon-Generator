#include <omp.h>
#include <popt.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <random>
#include "./Classes/Classes.hpp"

#ifdef COLOR
    const char* redText = "\033[1;31m";
    const char* resetText = "\033[0m";
#else
    const char* redText = "";
    const char* resetText = "";
#endif

using namespace std;

// Default values of parameters
int number_of_polygons = 1, verbose = 0, profiling = 0, graph = 0, dist_analysis = 0;
char *algorithm = NULL, *filename = NULL;

double timer;
long fileSize;
unsigned validPolygons = 0;

extern float Scale; // Canvas size

Polygon *polygons;

// Distribution for number of vertices in the polygon
default_random_engine generator(clock());
uniform_int_distribution<unsigned> distribution(10,500); // This program generates polygons with 10 to 500 vertices. Change these bounds for larger polygons

int main(int argc, const char** argv){ 
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    srand(time(0));
    int choice = rand()%5 + 1, algoChoice = -1;
    Scale = 1000; // Default canvas size
    static struct poptOption options[] = { 
        { "number_of_polygons", 'n',POPT_ARG_INT, &number_of_polygons, 0, "Set n = number of polygons that needs to be generated. [Default : n = 1]", "NUM" },
        { "verbose", 'v',POPT_ARG_INT, &verbose, 0, "Set v = 1 for verbose output (will slow down the program by some time)", "NUM" },
        { "algorithm", 'a',POPT_ARG_STRING, &algorithm, 0, "Set a = polar or spacePartition or naivePoly to select the algorithm used to generate the polygons", "STR" },
        { "graph", 'g', POPT_ARG_INT, &graph, 0, "Set g = 1 to graph the generated polygons onto a single canvas (using OpenGL)", "NUM" },
        { "profiling", 'p', POPT_ARG_INT, &profiling, 0, "Set p = 1 for profiling mode", "NUM"},
        { "filename", 'f', POPT_ARG_STRING, &filename, 0, "Enter the filename to which the generated polygons are to be written to in WKT format. [Default : map.wkt]", "STR"},
        { "distribution", 'd', POPT_ARG_INT, &dist_analysis, 0, "Set d = 1 for obtaining an analysis of the distribution of the polygons generated", "NUM"},
        { "canvas_size", 'c', POPT_ARG_FLOAT, &Scale, 0, "Set the canvas size within which all the polygons will be generated. [Default : c = 1000]", "NUM"},
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
        fprintf(stderr, "%spolygonGenerator: %s -- \'%s\'%s\n", redText, poptStrerror(c), poptBadOption(poptCONT, POPT_BADOPTION_NOALIAS), resetText);
        poptPrintUsage(poptCONT, stderr, 0);
        return 1;
    }

    const unsigned numAlgos = 3;
    typedef void (Polygon::*FunctionPointers)(bool, int);
    FunctionPointers generators[] = {&Polygon::Generator1, &Polygon::Generator2, &Polygon::Generator3}, currentGenerator;
    const char *validAlgos[] = {"Polar", "SpacePartition", "NaivePoly"};
    const char *algoFullNames[] = {"Polar", "Space Partition", "Naive Polygon Generation"};
    
    for(unsigned i = 0; i < numAlgos; ++i) {
        if(!strcasecmp(algorithm, validAlgos[i])) {
            algoChoice = i;
            currentGenerator = generators[i];
            break;
        }
    }

    if(algoChoice < 0){
        fprintf(stderr, "%spolygonGenerator: Invalid algorithm name.\npolygonGenerator: Try './bin/polygonGenerator -?' for more information.%s\n", redText, resetText);
        exit(1);
    }
    
    polygons = new Polygon[number_of_polygons];  // Creating an array of polygons
    
    if(profiling) verbose = false;

    if(!profiling && graph) printf("Graph scale = %f\n", Scale);

    pthread_t writerThread,   // This thread writes the polygons to the file in WKT format
              graphicsThread; // This thread plots the polygons onto a canvas using openGL
    int ret = pthread_create(&writerThread, NULL, writer, (void*)&number_of_polygons);
    if(ret) fprintf(stderr, "%sThere was an error launching the writer thread.\nThe error returned by pthread_create() is %s%s\n", redText, strerror(ret), resetText);
    
    start_timer(total);

    #pragma omp parallel for
    for(int i = 0; i < number_of_polygons; i++){
        polygons[i] = Polygon(distribution(generator));
        (polygons[i].*currentGenerator)(verbose, choice);
        #ifdef CHECKVALIDITY
            bool isPolyValid = polygons[i].validityCheck();
            if(!isPolyValid){
                fprintf(stderr, "%s                  Algorithm used: %s.%s\n\n", redText, algoFullNames[algoChoice], resetText);
                polygons[i].valid = false;                
            }
        #endif
        polygons[i].processing = false;
    }

    end_timer(total, timer);
    
    if(verbose){
        const char *distributions[25] = {"Uniform Distribution", "Binomial Distribution", "Geometric Distribution", "Poisson Distribution", "Normal Distribution"};
        printf("\nSampling from %s\n", distributions[choice - 1]);
    }  
    if(!profiling) printf("Writing the polygons to the file... ");
    
    // Plotting generated polygons onto a single canvas
    if(graph){
        ret = pthread_create(&graphicsThread, NULL, GraphicsInit, NULL);
        if(ret) fprintf(stderr, "%sThere was an error launching the graphics thread.\nThe error returned by pthread_create() is %s%s\n", redText, strerror(ret), resetText);
    } 
    
    pthread_join(writerThread, NULL);
    if(!profiling) {
        printf("Done\nFile size = %lu B\n", fileSize);
            printf("Total time taken for generating %u valid polygons using %s algorithm is %lf s\n", validPolygons, algoFullNames[algoChoice], timer);
    } 
    else printf("%u, %lf, %lu, %s\n", validPolygons, timer, fileSize, algorithm);
    
    if(graph) pthread_join(graphicsThread, NULL);

    // Distribution plot for generated map of polygons
    if(dist_analysis){
        printf("Plotting the distribution of the generated polygons...\n");
        // Read from user defined output file if filename not null. Otherwise, read from default output file map.wkt
        if(filename != NULL) execlp("python3", "python3", "Distribution.py", filename, (char*) NULL);
        else execlp("python3", "python3", "Distribution.py", (char*) NULL);
    }
    return 0;
}