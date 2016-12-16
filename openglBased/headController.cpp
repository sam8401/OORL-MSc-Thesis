#include "headController.h"

// variables copied from controller.cpp 
vector<Vec2f> lstDirns ;
unsigned int t0 = 1 ; // the global time variable
float dt = 0.1 ; //time step for computation it was .1
float DAMP = 100 ;  // DAMP = 100 ;
int tUpdate = 4 ;  // WEIRD, it should be 40 in windows
unsigned int MAX_COUNT = 65535 ; // just some huge value
float w = 10 ;
int win_x = 600 , win_y = 600 ; 

//string basePath ="C:\\Users\\suman.mukherjee\\Dropbox\\Documents\\MSc_Essay\\project\\windowsVer\\Debug\\files\\" ; // Windows
string basePath = "files/" ; // Linux 
string tempStr ;

// Constant Variables 

float limit_x ;   // sprite drawing widths
float limit_y  ;

int lastChecked = 0 ; // variable to track checking of boundary
int th_2 = 20 ;
int val = 0;
float h = 30 ; // grid width was 0.3
Vec2f pPassesnger(4,4) ;
int gridSize =120 ;   // the size of the grid
Vec2f v1,v2 ; // temporary vectors to be used
int toDrawState ;

 void checkCollison(agent * x) ;

 // Other variables for temporary usage
 

 void headController :: initScenery()
 {
	 vector<Vec2f>  x; 
	 tempStr = basePath +"wall_positions2.txt" ;
	readData(&x,tempStr.c_str());
	//cout << x[0] << endl << x[1] << endl ;
	for(int i=0;i<x.size()/2;i++)
		lstWalls.push_back(new wall(x[2*i], x[2*i+1])) ;
	cout << "Read Walls ........\n" ;
	// possible passenger start point and destinations
	  tempStr = basePath +"destinations.txt" ;
	 readData(&locations, tempStr.c_str());
	 cout << "Read start positions ....\n" ; // << locations.size() << endl ;
	 //cout << theGrid->m_x << endl ;
 }



 void headController :: initAgentStuff()
 {
	 tempStr = basePath + "directionsSimple.txt" ;
	 readData(&lstDirns,tempStr.c_str()); 
	cout << "Read global directions .....\n" ;
	//for(int i=0;i<4;i++)
		//cout << lstDirns[i][0] << " " << lstDirns[i][1] << endl ;
	taxi = new agent() ; 
	objpassenger = new passenger(0);
	 //m_learn = new oorl_learner(7,this);  // moved out to controller.cpp 
	 //m_learn->print_relations() ;
 }

 void headController :: InitSystem()
 {
	pDes = 3; // it is now a index for locations 
	theGrid = new grid(gridSize,gridSize); 
	//isLearningMode = learningMode?true:false ;	 
	initAgentStuff(); 
	initScenery();  
	//theGrid->addWallInfo(lstWalls) ;   <-- don't need to do this explicitly 
	//theGrid->printGrid();
	taxi->m_pos = theGrid->worldFromGrid(Vec2f(1,1)) ;
 }

 


 void headController :: Update(int  q[])
 {
		/*
		if(t0 % tUpdate == 0)
		{  
			checkCollison(taxi);
			taxi->update();
			// temporary "go to target" section
			if(magnitude(taxi->m_target-taxi->m_pos)>0.0001)
			{
				taxi->m_pos += (DAMP*dt)*taxi->m_vel ;
			}
			// check if passenger in taxi
			if(objpassenger->in_taxi)
				objpassenger->index = -1 ;
			if(!objpassenger->in_taxi && similar_vectors( theGrid->gridFromWorld(taxi->m_pos),locations[objpassenger->index]))
			{
				objpassenger->in_taxi = true ;
			}
			
		}
		t0++ ;
		t0 %= MAX_COUNT ;
		*/
		// UNCOMMENT ABOVE AND COMMENT BELOW TO RESUME NORMAL OPERATION
		
		taxi->m_pos= theGrid->worldFromGrid(Vec2f(q[0],q[1])) ;
		pDes = getIndex(Vec2f(q[2],q[3]), locations)  ;
		objpassenger->index = getIndex(Vec2f(q[4],q[5]),locations);
		objpassenger->in_taxi = q[6] ;
		glutPostRedisplay ();
 }


void headController :: DrawObjects()
{
	for(int i=0;i<lstWalls.size();i++)
		lstWalls[i]->drawWall() ; 
	// draw passenger
	if(!objpassenger->in_taxi)
	{
		glColor3f(0.3,0.3,0.3) ;
		v1 = theGrid->worldFromGrid(locations[objpassenger->index]);
		glRectf(v1[0] - w, v1[1]-w, v1[0]+w, v1[1]+w) ;
	}
	taxi->draw(Vec3f(1,1,1));  
	// draw destination 
	Vec2f pd = theGrid->worldFromGrid(locations[pDes]);
	glColor3f(1.0,0.5,0.0) ;
	glRectf(pd[0] - th_2/2, pd[1]-th_2/2, pd[0]+th_2/2, pd[1]+th_2/2) ;					
}

 // other functions used by headCOntroller
 void checkCollison(agent * x)
{
	/*checking collision with boundary of the walls*/
	 if(t0 - lastChecked < 10*tUpdate) /*boundary has been checked recently*/
		 return ;
	if( (win_x - abs(x->m_pos[0])) < th_2  || (win_y- abs(x->m_pos[1])) < th_2)
	{
		lastChecked = t0 ; 
		x->isMoving = false ;  
		return ;
	}  
	/*if control flow has reached here, its OK*/
	x->isMoving = true ;
	
}