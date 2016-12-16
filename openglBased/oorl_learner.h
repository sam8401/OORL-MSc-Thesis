#include "relation.h"
//--- also including headcontroller.h to be able to be call mainControl
#include "headController.h"


// structure for storing attributes (MIGHT NOT NEED THIS IN FUTURE)
struct nvpair 
{
	string name ;
	int value; 
	// constructor for struct (It works !)
	nvpair(string pname, int pvalue ) : name(pname),value(pvalue) {} 
};


// structure for predictions
struct prediction 
{
	string model ; // a string like the one cond(s) returns
	nvpair * effect  ; 
	prediction(string pm, nvpair * pe): model(pm), effect(pe) {}
} ;


// structure for pred(a,att,type)
struct pred 
{
	string a ;   // action
	nvpair * att ; // attribute
	string type ;  // effect type
	vector<prediction *> lstPred ; // list of all models and effects that fall under the above trio 
	
	pred(string pa, nvpair * patt , string ptype):a(pa), att(patt), type(ptype)
	{
	}
};



class oorl_learner 
{
	vector<relation *> relations ; // global list of all relations
	//vector<nvpair *> effects
	vector<string> eff_types ;		
	vector<nvpair *> attributes ;
	vector<string> actions ;
	vector<pred *> P ; // set of all active predictions
	 
	float ** Q ; // state-action model
	//int ** T; // state-transition model, deterministic
	//int * pi ; // policy vector for available states
	//float * V ; // Value vector for states
	//float* V_prev ; // helper to store last iteration V's values 
	
	vector<string> Fa ; // failure list for all actions ; (concatanated strings for each action)
	int nstates ;
	int nactions ; 
	int nAttr ; 
	int n ; // number of distinct relations without counting negations
	
	int m_explore  ;
	
public :	// NOTE: make the next three variable PRIVATE
	headController * world ;
	int ** state_mat ;  // for access publicly by Controller
	int s_max ; // the state that is returned in case no other state is obtained from 
	

	oorl_learner(int , int , headController* &); // nrelations, nattr, world
	// INIT FUNCTIONS
	void initActions();
	void initStates();  // taxi_x, taxi_y, dest_x, dest_y, passgr_x, passgr_y, in_taxi(boolean)
	void initAttributes();
	void load_relations(); // load the global set of "relations" applicable to the model
	// OPERATIVE FUNCTIONS (MIGHT NOT NEED)
	void print_relations(); // just print out the shit, will you ?
	
	//  OORL HELPER FUNCTIONS
	string cond(int pState) ;  // Update : CHANGED it to just take state number which should be processed  
	vector<nvpair *> eff_att(int natt, int s, int s_);
	bool compareCondStr(string s1, string s2);
	string commutativeOp(string , string) ;
	int getStateFromWorld();
	int getStateNumber(int []) ;  // gets the vector of int containing x,y,d,p
	bool FailureStateActionCombo(int s, int a);
	void updateFailure(string s, int  a) ;
	nvpair *  findMatchingModel(string a, nvpair * att, string eff_type, string pmodel);
	nvpair * findMatchingModel(string a, nvpair * att, string eff_type, nvpair * peffect);  // overloaded method
	bool isCompatible(vector<nvpair *> E, int attrValue); 
	int applyEffect(int att, nvpair * E) ; // apply effect E to attribute att (not index)
	
	// Usual RL functions
	
	float getMaxQ(int) ; // return the maximum value i.e.  max_a Q(s,a)
	int getBestAction(int) ; // return appropriate action, given a state, simple max operation
	int getReward(int s, int a) ; // get a reward based on the current state and the selected action
	int carryOutAction(int s , int a); // perform the given action to know the next state
	void valueIteration();  // update Q for next action to be taken
	void valueIteration2(int);
	void saveQ() ;	// save a particular Q to disk
	void loadQ() ;
	void runController() ; // runs the world starting from state pstart
	
	// OORL MAIN FUNCTIONS
	void DOORMAX_main(int numIter) ; // takes the number of iterations to be done as input
	int predictTransition(int state, int action) ; // takes the index of the particular s,a and predicts next state (s)
	void addExperience(int state, int action, int staten, int k) ; // not so sure what "k" is
	
	
	// Temporary functions 
	void temp_test1();
};
