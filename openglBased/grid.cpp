#include "grid.h"

float m,c ;  // line parameters
Vec2f  x0, point;

grid :: grid (float gridX, float gridY)
{
	m_step = Vec2f(gridX,gridY);
	m_x = win_x/m_step[0];
	m_y  = win_y/m_step[1] ;
	//cout << m_x << m_y << "\n" ;	 
}

// Return the arena co-ordinates given the grid co-ordinates
Vec2f grid::worldFromGrid(Vec2f x)
{
	Vec2f t ; 
	t[0] = 2*m_step[0]*x[0] - win_x + m_step[0] ;
	t[1] = win_y - 2*m_step[1]*x[1]  - m_step[1];
	return t ;
}

// Return the appropriate matrix indices for a given Arena Point
Vec2f grid::gridFromWorld(Vec2f x)
{
	Vec2f t ;
	t[0] = ceil((x[0] + win_x)/(2*m_step[0])) ;
	t[1] =  ceil((win_y - x[1])/(2*m_step[1])) ;
	t[0] -= 1 ; t[1] -= 1 ;
	return t ; 
}


