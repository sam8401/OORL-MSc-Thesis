

// inclusion of windwos.h is a must to run in windows

#ifdef _WIN32
	#include <windows.h>
#endif
// comment above if trying to run in Linux
#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glut.h"
#include "gfx/vec2.h"
#include "gfx/vec3.h"
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h> // for srand(time(0)) ;

using namespace std ;

// extern variables 


extern vector<Vec2f > lstDirns ;
extern GLuint texHandle[3] ; 
extern unsigned int  t0 ;
extern float dt  ; 
extern float DAMP  ;  
extern int win_x,win_y ;
extern int gridSize ;
extern float gtile_x ;
extern float  gtile_y ;
extern int tUpdate ; // agent and human update frequencys
extern bool isLearningMode ;
extern bool gridMode ; 
extern string basePath ; // to handle linux/windows issues
// utility functions 

/* Read contents fron a text file and return information*/ 
void readData(vector<Vec2f> *, const char *) ;
/*Return the magnitude of vector X*/
float magnitude(Vec2f x) ;
// check similarity of vectors
bool similar_vectors(Vec2f x, Vec2f y) ;
/*split the string object based on the character char */
vector<string> split_string(string , char ) ;
// search for a vec2f in a vector of vec2fs and return index
int getIndex(Vec2f val, vector<Vec2f> list) ;