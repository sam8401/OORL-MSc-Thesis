#include "wall.h"

int dirx = 100 ; 
int diry = 60 ;
int tx = 70 ; int ty = 74 ;
int count ;
float temp_val, extra ;

wall :: wall(Vec2f p_A, Vec2f p_B)
{
	m_A = p_A ;
	m_B = p_B ;
	m_sign[0]  = -1 ;
	if(m_B[1] > m_A[1])
		m_sign[1] = -1 ;
	else
		m_sign[1] = 1 ;
	
}


void wall :: drawWall()
{ 
	glColor3f(1.0,1.0,1.0);
	glBegin(GL_LINES) ;
	glVertex2f(m_A[0],m_A[1]);
	glVertex2f(m_B[0],m_B[1]);
	glEnd();
		
}