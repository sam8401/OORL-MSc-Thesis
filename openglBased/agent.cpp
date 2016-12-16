#include "agent.h"

// utility functions, later add these to another common file 

int index ;
float minm   ;
Vec2f x1 ;  




int th_1 = 40 ;
//  WILL USE THIS CONSTRUCTOR
agent::agent()
{
	m_pos = Vec2f(0,0); 
	m_spriteID = 0 ;
	// NEW stuff for TAXI problem
	m_spriteID = 0 ; // initial velocity change this 
	m_pos = Vec2f(0,0);
	//m_learn = new oorl_learner(7); // initiate the OORL learning module // moved to headController.h
	isRL = false ;
	//
	isMoving = true ;
	initBasics();
	update();
}

void agent::  initBasics()
{
	m_numPoses = 10; // number of poses for each possible type of movement
	m_numSprites = 4  ; // eight possible directions of movement
	m_spriteHeight = 405 ;
	m_spriteWidth = 375;
	m_spriteIndex = 0 ;  // intialize sprite count
	hx = 1.0/(float)m_numPoses ; 
	hy = 1.0/(float)m_numSprites ;
	pos_x = hx * m_spriteWidth ;
	pos_y = hy*m_spriteHeight ;
	m_t = tUpdate  ; // change this later
}

// constructor for BOT
// also initiates the learner class 
agent :: agent(const Vec2f p_pos, int  p_vel, bool pisRL) 
{
	//m_learn = new learner((thisGrid->m_x*thisGrid->m_y),8,isLearningMode,thisGrid);
	m_pos = p_pos ;  
	m_spriteID = p_vel ;   
	isMoving = false ;
	isRL = pisRL ;
	//m_tag = "bot" ;
	m_target = Vec2f(0,0); // this should be updated
	initBasics();
	update();
}


void agent:: draw(Vec3f m_col)
{ 
	
		glColor3f(0.0,0.0,0.0);
		glRectf(m_pos[0]-th_1/2,m_pos[1] - th_1/2,m_pos[0]+th_1/2,m_pos[1] + th_1/2); 
	
	
	// Draw target
	//glRectf(m_target[0]-th_1/2,m_target[1] - th_1/2,m_target[0]+th_1/2,m_target[1] + th_1/2); 
}
/*lower level Agent function, called by Update()*/
void agent::takeAction()
{
	
	isMoving  = true  ; 
	updateVelocity() ;   // based upon target 
	if(t0%3*m_t == 0)
	{
		m_target = Vec2f(0,0);
	}
 	
}

/*Function to ensure that the agent doesn't escape the Arena*/
/*
void agent:: checkBoundary()
{
	 if(t0 - lastChecked < 10*tUpdate)  
		 return ;
	if( (win_x - abs(m_pos[0])) < th_1  || (win_y- abs(m_pos[1])) < th_1)
	{
		lastChecked = t0 ;
		if(!m_tag.compare("human"))
		{ 
			isMoving = false ; 
		}
		else // its a fucking BOT 
		{
		//cout << win_x << " " <<  abs(m_pos[0]) << "\n" ;
			m_spriteID += 4 ; // reverse Direction
		}
	}
		
} 
*/


/*Function to update velocity based upon given target to head to, called takeAction()*/
void agent::updateVelocity()
{ 
	 
}

/*Update function, called by controller*/
// takes care of m_target changes etc and main goal of the agent
void agent::update()
{	
	if(t0 % m_t == 0)
		takeAction(); 
	if(isMoving)
	{
		m_vel = lstDirns[m_spriteID]; 
		m_spriteIndex++ ;
		m_spriteIndex  %=  m_numPoses;
	}
	
	
}


 