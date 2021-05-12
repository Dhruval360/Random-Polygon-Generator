#include<GL/freeglut.h>
#include<GL/gl.h>
#include<pthread.h>
#include<random>
#include"Polygon.hpp"

float Scale; // The default coordinate system in openGL is -1 to +1 (the entire screen height and width will be 2 units in length)

extern Polygon *polygons;
extern unsigned number_of_polygons;

void Plotter() {
    glPointSize(1.0);
    glClearColor(0.0, 0.0, 0.0, 1.0); 
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);

    for(unsigned j = 0; j < number_of_polygons; j++) {
        glBegin(GL_LINE_LOOP);
        glColor3f(polygons[j].color[0], polygons[j].color[1], polygons[j].color[2]);
        for(unsigned i = 0; i < polygons[j].numVertices; i++) glVertex2f(polygons[j].coordinates[i].first/Scale, polygons[j].coordinates[i].second/Scale); 
        glVertex2f(polygons[j].coordinates[0].first/Scale, polygons[j].coordinates[0].second/Scale);
        glEnd(); 

        glPointSize(2);  
        glBegin(GL_POINTS);
        glColor3f(1, 1, 1);
        for(unsigned i = 0; i < polygons[j].numVertices; i++) glVertex2f(polygons[j].coordinates[i].first/Scale, polygons[j].coordinates[i].second/Scale); 
        glEnd();

        glFlush();
    }
}

void* GraphicsInit(void *arg) {
    // Initialize GLUT and process user parameters
    int argc = 1;
    char *argv[1] = {(char*)"Something"};
    glutInit(&argc, argv);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL - Polygon Plotter");
    glutDisplayFunc(Plotter);
    glutMainLoop();
    return NULL;
}