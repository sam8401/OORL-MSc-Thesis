// External Libraries


#include "core.h"



class agent 
{
	public :
	Vec2f m_pos;
	Vec2f m_vel ; 
	Vec2f m_target  ;
	//oorl_learner *m_learn ;  // moved to headCOntroller.h
	
	int m_t ; // decision frequency
	//string m_tag ; // to differentiate between human and bot
	bool isMoving ;
	bool isRL ; // tag to differentiate between RL agent and bayesian Agent
	//Vec3f  m_col ; // colour of the sprite
	 // 8 possible directions of movement
	// Sprite dimensions
	int m_spriteWidth ;
	int m_spriteHeight ;
	int m_numPoses ;
	int m_numSprites ; 
	
	int m_spriteIndex ; // which pose? takes values from 0 to m_numPoses-1  (columns)
	int m_spriteID ; // which direction? one of 0 to m_numSprites-1 (rows)
	float hx,hy,lx,ly ,pos_x,pos_y;  //// vairables for drawing the sprite
	
	agent(const Vec2f  ,int , bool  );
	agent();
	void initBasics();
	void update();
	void takeAction();
	void updateVelocity();
	
	//void checkBoundary();    // to be taken care of by the main controller
	void draw(Vec3f m_col); // render the agent using the corresponding sprite
};
