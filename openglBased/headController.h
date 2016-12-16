#include "agent.h"
#include "grid.h"
#include "passenger.h"



class headController {
	public :   // Worry about making these private later, also add methods to access specific variables
	grid * theGrid  ; 
	agent * taxi ; 
	passenger * objpassenger ;
	vector<wall *> lstWalls ;
	vector<Vec2f> locations ;  // possible values for passenger start and destinations (in grid co-ord)
	int pDes ;
	//oorl_learner *m_learn ; // moved from agent.h  // and moved out to controller.cpp
	 
	
	//friend class oorl_learner ;  <-- could do this and make above variables PRIVATE 
	bool isLearningMode ;
	

	// init functions
	void initScenery();
	void initAgentStuff();
	void InitSystem(); // added extra_integer to temporarily show state 
	 
	//void checkCollison(agent * x) ;
	void Update(int  inp[]);
	// drawing functions
	void DrawObjects() ; // 
	// uses agent, walls, destination,passenger information and returns "condition string"
	// takes the list of relations to generate the "condition string"
	 // called upon by oorl_learner to get the "condition string"

};
