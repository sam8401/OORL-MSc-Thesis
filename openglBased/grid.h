
 #include "wall.h"



class grid {
	public :
		
	int m_x ; // the matrix dimensions
	int m_y;  
	Vec2f m_step ;

	


	grid(float gridX, float gridY) ;
	
	Vec2f worldFromGrid(Vec2f) ;
	Vec2f gridFromWorld(Vec2f x);
	
};