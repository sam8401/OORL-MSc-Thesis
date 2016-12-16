#include "core.h"

extern GLuint texHandle[3] ; 

// each wall -> a row in the constraint matrix of type Ax>b | Ax< b 

class wall {
	public : 
	Vec2f m_A ;  // the two ends of the wall
	Vec2f m_B ; 
	Vec2f m_sign ; // to be initialized based upon val_A and val_B  (only used )
	wall(Vec2f p_A, Vec2f p_B); 
	void drawWall();
	 
	
} ;