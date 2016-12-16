#include "oorl_learner.h" 
#include <sstream> // for integer to string conversion
#include <ctime> // seed random number generator with a time based value
#include <unistd.h> // for usleep() function called in 

#define MAX_GLOBAL_ITER 10 // maximum observed was 11 , might not need this
#define MIN_VALUE -9999 // get rid of this !

vector<relation*>  read3Data(const char *, int) ;
int threshold =60 ; 


// TEMPORARY VARIABLES USED THROUGHOUT THE CODE

int x,y,d,p ;
int dx,dy,px,py ; // redundant ones 
int X,Y,D,l ;
int details[7] ; 
int info[4] ;

float bestf ;
int besti ;
int itr = 0 ;

string s_cond ;
vector<nvpair *> effectlist; 
prediction * pred1 ;
nvpair * nvp ;
bool tbool ;
//string basePath2 ="C:\\Users\\suman.mukherjee\\Dropbox\\Documents\\MSc_Essay\\project\\windowsVer\\Debug\\files\\" ; // Windows
string basePath2 = "files/" ; // Linux 
string tempStr2 ;

//  globals to run the controller 
int s_sim  ;
int a_sim ;
int q[7] ;

// init actions and the failure state of each action ;
void oorl_learner ::  initActions()
{
		
	actions.push_back("north");
	actions.push_back("south");
	actions.push_back("east");
	actions.push_back("west");
	actions.push_back("pickup");
	actions.push_back("dropoff");
	nactions = actions.size() ; 			
}

void oorl_learner :: initStates() 
{
	int x= 0 ;
	int y = 0 ;
	int p, d ;
	p = d = 0;
	grid * t  = world->theGrid ;
	
	nstates = world->theGrid->m_x * world->theGrid->m_y * world->locations.size()* (world->locations.size()+1) ;
	//cout << nstates << endl ;
	state_mat = new int* [nstates] ;
	
	for(int i=0;i<nstates;i++)
		state_mat[i] = new int[nAttr]; 
	for(int i=0;i<nstates;i++)
	{
		 
		//states.push_back(new state(Vec2f(x,y),d,p)) ;
		state_mat[i][0] = x ; state_mat[i][1] = y ; 
		state_mat[i][2] = world->locations[d][0] ; state_mat[i][3] =  world->locations[d][1];
		
		if(p >= world->locations.size())
		{
			state_mat[i][4] = x ; state_mat[i][5] =  y; // passenger is in taxi 
			state_mat[i][6] = 1 ;   // in_taxi is true
		}
		else
		{
			
			state_mat[i][4] = world->locations[p][0] ; state_mat[i][5] =  world->locations[p][1];
			state_mat[i][6] = 0 ;
		}
		if(x == t->m_x-1)
		{
			x = 0; 
			if(y == t->m_y-1)
			{
				// reset x,y to (0,0)
				y  = 0 ;
				 // make changes to p and d
				if(d == world->locations.size()-1)
				{
					d = 0 ;
					p++  ;
				}
				else
					d++ ; 
			}
			else
				y ++ ;
		}
		else
			x ++ ;
		
	} 
	 
}

// THIS might not be needed, could just use the columns of state matrix
 
// name = meaningful name , value = column number in the state matrix
void oorl_learner :: initAttributes()
{
	
	attributes.push_back(new nvpair("taxi_x",0)) ;
	attributes.push_back(new nvpair("taxi_y",1)) ;
	attributes.push_back(new nvpair("dest_x",2)) ;
	attributes.push_back(new nvpair("dest_y",3)) ; 
	attributes.push_back(new nvpair("passg_x",4)) ;
	attributes.push_back(new nvpair("passg_y",5)) ;
	attributes.push_back(new nvpair("in_taxi",6)) ;
}

oorl_learner::oorl_learner(int p, int pAttr, headController * & obj)
{
	n = p ;
	world = obj ;
	nAttr = pAttr ;
	srand(time(0)) ; 
	// initialize OORL related stuff
	initActions() ;
	initStates() ;
	initAttributes();
	load_relations();
	// init effect types , still have to take care of EFFECT THOUGH
	eff_types.push_back("increment") ;
	eff_types.push_back("set-to") ;
	
	// allocate space to Q, state-transition , Policy and Value variables
	Q = new float *[nstates] ;
	// initiate matrix Q and state-transition model
	for(int i=0;i<nstates;i++)
	{
		Q[i] = new float[nactions] ;
	}
	for(int i=0;i<nstates;i++)
	{
		for(int j=0;j<nactions;j++)
		{
			Q[i][j] = 1 ; // initially all state/action combination should look fruitful to the agent (MAKE THIS MORE) 
		}
	} 
	
	X = world->theGrid->m_x ;
	Y = world->theGrid->m_y ;
	D = world->locations.size() ;
	d = world->pDes ;
	// initialize s_max ;
	s_max = D*(X*Y*D) + world->pDes*(X*Y) + world->locations[d][1]*X + world->locations[d][0] ;
}

void oorl_learner :: load_relations()
{
	tempStr2 = basePath2 + "relation_list.txt" ;
	relations = read3Data(tempStr2.c_str(),n); 
}

void oorl_learner:: print_relations()
{
	for(int i=0;i<n;i++)      // not adding negatives but printing the negative in "relation.cpp"
	{
		relations[i]->print();
	}
}

vector<relation *> read3Data(const char * filename, int n)
{ 
	string line,p1,p2,p3 ;
	vector<relation *> result;
	vector<string> temp ;
	
	ifstream infile; 
	infile.open(filename, ifstream::in) ;
	if(infile!= NULL)
	{
		//while(infile.good())  // This loop could be alternatively ran for 'n' times
		for(int i=0;i<n;i++)	// This loop could alternatively ran until file is readable
		{ 
			getline(infile,line);
			temp = split_string(line,',');
			//cout << temp[0] << " " << temp[1] << " " << temp[2] << endl ;
			result.push_back(new relation(temp[0],temp[1],temp[2]));
			//result.push_back(new relation("n"+temp[0],temp[1],temp[2])); // adding the negation of the relation
		}
		infile.close();
	}
	return result ;
}




bool check_str(const char *s1, char *s2, int l)
{
	bool r = true ;
	for(int i=0;i<l;i++)
	{
		if(s1[i] != s2[i])
		{
			r = false ; break ;
		}
	}
	return r ;
} 


 

// takes agent, walls, destination,passenger information and returns "condition string" of the form "1000101.."

// UPDATE: it now takes only state numbers, and uses the numbers directly to get out the resultant string
// touchN, touchS, touchE, touchW,
  
string oorl_learner :: cond(int pState) 
{
	string result ;
	int i, temp;
	int wall_x, wall_y ;
	Vec2f tvec,tvec1 ;
	string r ;
	string s1 = "taxi" ;
	char * s2 = "wall" ;
	char * s3 = "passenger" ;
	char * s4 = "destination" ;
	// currently assuming that order in the text file is all that matterss
	
	x = state_mat[pState][0] ; y  = state_mat[pState][1] ;
	dx  = state_mat[pState][2] ; dy  = state_mat[pState][3] ; 
	px  = state_mat[pState][4] ; py  = state_mat[pState][5] ; 
	
	for(int i=0;i<relations.size();i++)
	{
		//cout << "I was here " << endl ;
		if(relations[i]->obj1 == s1)
		{
			
			if(check_str(relations[i]->obj2.c_str(),s2,4))
			{
				// ALL RELATIONS BETWEEN THE TAXI AND WALL, THE TRICKY ONES 
				if(relations[i]->name=="touchN")
				{
				// wall on north
					if(y == 0)
						result.append("1");
					else
						result.append("0");
				} 
				else if(relations[i]->name=="touchS")
				{
				// wall on south
					if(y == world->theGrid->m_y-1)
						result.append("1");
					else
						result.append("0");
				} 
				// wall on east
				else if(relations[i]->name =="touchE")
				{ 
					r = "0" ;
					for(int k=0;k<world->lstWalls.size();k++)
					{ 
						tvec = world->theGrid->gridFromWorld(world->lstWalls[k]->m_A) ;
						tvec1 = world->theGrid->gridFromWorld(world->lstWalls[k]->m_B) ;  
						 // sign checks on y will be opposite in this case 
						if(y >= tvec[1] &&  y <= tvec1[1]  && tvec[0] == x)
						{
							r = "1" ; break ;
						}
						
					}
					if(r=="0" && x==world->theGrid->m_x-1)  // check proximity to boundaries
						r = "1" ; 
					result.append(r);
				} 
				// wall on west
				else
				{
					r = "0" ;
					for(int k=0;k<world->lstWalls.size();k++)
					{ 
						tvec = world->theGrid->gridFromWorld(world->lstWalls[k]->m_A) ;
						tvec1 = world->theGrid->gridFromWorld(world->lstWalls[k]->m_B) ;  
						 
						if(y >= tvec[1] &&  y <= tvec1[1]  && tvec[0] == x-1)
						{
							r = "1" ; break ;
						} 
					}
					if(r=="0" && x==0)  // check proximity to boundaries
						r = "1" ; 
					result.append(r);
				}    
			} // end if wall 
			
			// relations between TAXI AND PASSENGER
			
			// this will be always be false once passenger gets in the TAXI
			// will be true if taxi comes near by the passenger
			else if (check_str(relations[i]->obj2.c_str(),s3,9)) 
			{
				//cout << relations[i]->obj1<< "," << relations[i]->obj2 << endl ;
				(!state_mat[pState][6]  && similar_vectors(Vec2f(x,y),Vec2f(px,py)))?result.append("1"):result.append("0"); 
			} 
			// relations between TAXI and DESTINATION
			// pDestination is already in grid co-ordinates
			else if (check_str(relations[i]->obj2.c_str(),s4,11))
			{
				//cout << relations[i]->obj1<< "," << relations[i]->obj2 << endl ;
				similar_vectors( Vec2f(x,y),Vec2f(dx,dy))?
				result.append("1"):result.append("0"); 
			} 
		} // end if taxi
		else  // 
		{
			//cout << relations[i]->obj1<< "," << relations[i]->obj2 << endl ; 
			state_mat[pState][6]?result.append("1"):result.append("0"); 
			//world->objpassenger->in_taxi?result.append("1"):result.append("0"); 
		}
	} 
	return result ; 
}

// this, actually might very well be a simple mathematical operation that might not need a separate function 
// like this, but oh, well ..
int oorl_learner :: getStateFromWorld()
{	
	Vec2f  tvec ;
	tvec = world->theGrid->gridFromWorld(world->taxi->m_pos) ;
	x = tvec[0] ; y = tvec[1] ;
	d = world->pDes ;
	 if(world->objpassenger->in_taxi)
		 p = D ;
	 else
		 p = world->objpassenger->index ;
	 
	info[0] = x ; info[1] = y;
	info[2] = d; info[3] = p ;
	
	return getStateNumber(info);
}

int oorl_learner :: getStateNumber(int list[4])
{
	return (list[3]*(X*Y*D) + list[2]*(X*Y) + list[1]*X + list[0]) ;	
}

// return new state s' (after carrying out action)
int oorl_learner :: carryOutAction(int s , int a)
{
	//state_mat: taxi_x, taxi_y, dest_x, dest_y, passgr_x, passgr_y, in_taxi(boolean)
	// actions: north, south, east, west, pickup, dropoff
	////cond(s) returns -> touchN, touchS, touchE, touchW,
	//info[0] = x ; info[1] = y
	//info[2] = d; info[3] = p ;
	info[0] = state_mat[s][0] ; info[1] = state_mat[s][1];
	info[2] = getIndex(Vec2f(state_mat[s][2],state_mat[s][3]),world->locations) ;
	if(state_mat[s][6] == 1) 
		info[3] = D ;
	else
		info[3] = getIndex(Vec2f(state_mat[s][4],state_mat[s][5]),world->locations) ;
	

	
	string S = cond(s);
	switch(a)
	{
		case 0: // north  
		{
			if(S[0] == '1')
				return s ;
			else 
				info[1] -= 1 ;
			break ;
		}
		case 1: // south  
		{
			if(S[1] == '1')
				return s ;
			else
				info[1] += 1 ;
			break ;
		}
		case 2: //east 
		{
			if(S[2] == '1')
				return s ;
			else
				info[0] += 1 ;
			break ;
		}
		case 3: // west 
		{
			if(S[3] == '1')
				return s ;
			else
				info[0] -= 1 ;
			break ;
		}
		case 4:
		{
			if(state_mat[s][6] || info[0] != state_mat[s][4] || info[1] != state_mat[s][5])
				return s ;
			else
				// pickup success
				info[3] = D ;
			break ;
		}
		case 5:
		{
			if(!state_mat[s][6] || info[0] != state_mat[s][2] || info[1] != state_mat[s][3])
				return s ;
			else
				return s_max ;
			break ;
		}
			
	}

	return getStateNumber(info);
	
}

// get Maximum Q values for a given state
float oorl_learner :: getMaxQ(int s)
{
	bestf = Q[s][0] ;
	for(int i=0;i<nactions;i++)
	{
		if(Q[s][i]> bestf)
			bestf = Q[s][i] ;
	}
	return bestf ;
	
}
// get best action, similar to above but picks action 

int oorl_learner :: getBestAction(int s)
{
	besti = 0 ;
	 //p = rand()%10 + 1  ;
	//if( p<=7)
	//{
		for(int i=0;i<nactions;i++)
		{
			//cout << Q[s][i] << ",  " ;
			if(Q[s][i]> Q[s][besti])
				besti = i ;
		}
		cout << "Best action : " << besti << endl ;
	/*}
	else
	{
		besti = rand()%nactions ;
		cout << "\nRand Action: " << besti << endl ;
	}*/
	return besti ;
	
}


// Taken from the paper , from page 30 of the thesis
int oorl_learner :: getReward(int s, int a) 
{
	if(actions[a] == "dropoff")
	{
		// drop-off at the correct location with passenger in
		if(state_mat[s][6] && (state_mat[s][0] == state_mat[s][2]) && (state_mat[s][1] == state_mat[s][3]) )
			return 0 ;	// might want to increase this
		// drop-off at the wrong location
		else if((state_mat[s][0] != state_mat[s][2]) || (state_mat[s][1] != state_mat[s][3]) || !state_mat[s][6])
			return -10 ;
	}
	// pick up at the wrong location
	else if(actions[a] == "pickup" && (state_mat[s][0] != state_mat[s][4] || state_mat[s][1] != state_mat[s][5] ))
		return -10 ;
	// for any other aditional "step"
	else 
		return -1 ;
	
	
}

// return the 2-norm of difference of vectors v1 and v2 
// currently used by the function valueIteration()


// Infinite value iteration method to get a policy given T(s,a -> s')
void oorl_learner :: valueIteration()
{
	float temp ;
	bool changed;
	int countItr = 0 ;
	int i_ ;
	
	bool printed = false ;
	do 
	{
		changed = false ;
		for(int i = 0;i<nstates;i++ )
		{
			for(int a = 0;a<nactions;a++)
			{
				// UPDATE: if failure detected, ?
				 i_ = predictTransition(i,a) ; 
				temp  = getReward(i,a) + getMaxQ(i_) ;
				if(temp != Q[i][a])
				{
					changed = true ;
					if(!printed)
					{
						//cout << "Atleast changed once !"<< endl ;
						printed = true ;
					}
				}
				Q[i][a] = temp ;	 					
			}
		}
		countItr ++ ;
	}while(changed && countItr<MAX_GLOBAL_ITER);
	//cout << "Total Iterations: " <<  countItr <<endl ;
}

void oorl_learner :: valueIteration2(int s)
{
	int s_ ;
	for(int a = 0;a<nactions;a++)
	{
		 s_ = predictTransition(s,a) ; 
		Q[s][a] =  getReward(s,a) + getMaxQ(s_) ;
	}
}



void oorl_learner :: DOORMAX_main(int itr_max) 
{
	//int itr = 0 ;
	//int itr_max is now a parameter passed by the function
	int r,a,a_,s,s_,k ;
	// temporary 
	float m_gamma = 0.8;
	float m_alpha = 0.5 ; 
	float maxval ;
	//
	k = 500	; // not so sure about this
	int temp ; // delete this later
	for(int i=0;i<nactions;i++)
	{
		Fa.push_back("");
		for(int j=0;j<nAttr;j++)
		{
			for(int k=0;k<eff_types.size();k++)
			{
				P.push_back(new pred(actions[i],attributes[j],eff_types[k])) ; // lstPred is empty (should be, right?)
			} 
		}
	}
	
	s = rand()%nstates ; //  start with a random state
	 s = 97 ;
	
	while(s!=s_max && itr < itr_max) 
	{ 
		// STEP:1 : use value iteration  to update Q, use predict transition here
		cout << "Performing value iteration for iteration: " << itr << ".......\n" ;
		  
		valueIteration() ;
		
		// STEP-2: use Q to predict next action a and observe new s_
		cout << "Current State: " << s << endl ;
		a = getBestAction(s) ;
		s_ = carryOutAction(s,a) ; 
		cout << "New State: " << s_ << endl << endl ; 
		
		// STEP -3:  addExperience
		addExperience(s,a,s_, k) ;		// still no clue what "k" is !  
		s = s_ ;	
		itr++ ;
	}
	
}






// Helper : Function to do the commutative operation of two strings
string oorl_learner :: commutativeOp(string s1, string s2) 
{
	string result ;
	string temp ;
	for(int i=0;i<n;i++)
	{
		if(s1[i] == s2[i])
		{
			temp = s1[i] ;
			result.append(temp);
		} 
		else
			result.append("*") ;
	}
	return result ;
}



// Helper

bool oorl_learner :: compareCondStr(string s1, string s2)
{
	
	bool result = true ;
	for(int i=0;i<n;i++) // run for all characters
	{
		if(s1[i] != s2[i] && s1[i] != '*') //&& s2[i] != '*') // UPDATE: removed the last part of the check
		{
			result = false ;
			break ;
		}
	}
	return result ;
}




// Helper, check to see if cond(s) is in Fa(a)
bool oorl_learner :: FailureStateActionCombo(int s, int a)
{
	int i=0 ;
	s_cond = cond(s) ;
	bool result = false ;
	while(i<Fa[a].length())
	{
		if(compareCondStr(Fa[a].substr(i,n), s_cond))
		{
			result = true ;
			//cout << "Found failure state: " << s << " and action " << actions[a] << " combination " << endl ;
			break ;
		}
		i += n ;
	}
	return result ;
}

// Helper 
// for a given action, attribute and effect type combination look through the list of 
// predictions for a given model, pmodel 
nvpair * oorl_learner :: findMatchingModel(string pa, nvpair * patt, string ptype, string pmodel)
{
	//cout << "Called now !" << endl ;
	nvpair * result = new nvpair("-1",-1) ;
	ostringstream convert; // having to initialize this everytime, find a way out
	for(int i=0;i<P.size();i++)
	{
		//if(P[i]->a == pa && P[i]->att == patt && P[i]->type == ptype)
		// changed from above line, testing for equality in pointers woudn't work like that
		if(P[i]->a == pa && P[i]->att->name == patt->name && P[i]->type == ptype)	
		{
			convert << i ;
			result->name = convert.str() ;
			for(int j=0;j<P[i]->lstPred.size();j++)
			{
				if(compareCondStr(P[i]->lstPred[j]->model,pmodel))
				{
					result->value = j ;
					break ;
				}
			}
			break ;
		}
	}
	return result ;
}

// OVERLOADED METHOD, SAME AS ABOVE ... equality check against peffect instead 
// BUT RETURN TWO INDEXES, one for the location of corresponding pred in P 
// AND THE OTHER, location of corresponding prediction  in P->lstPred

nvpair * oorl_learner :: findMatchingModel(string pa, nvpair * patt, string ptype, nvpair * peffect)
{
	//cout << pa << " " << patt->name << " " << ptype << " " << peffect->name << endl ;
	nvpair * result = new nvpair("-1",-1) ;
	ostringstream convert;  // having to initialize this everytime, find a way out
	for(int i=0;i<P.size();i++)
	{
		if(P[i]->a == pa && P[i]->att->name == patt->name && P[i]->type == ptype)
		{
			convert << i ;
			result->name = convert.str() ;
			for(int j=0;j<P[i]->lstPred.size();j++)
			{
				if(P[i]->lstPred[j]->effect->name == peffect->name && P[i]->lstPred[j]->effect->value == peffect->value)
				{
					result->value = j ;
					break ;
				}
			}
			break ;
		}
	}
	return result ;
}


// gets an attribute att (not the index)  and applied the effect E
// possible updates :
int oorl_learner :: applyEffect(int att, nvpair * E)
{
	// write a case for each effect type
	if(E->name == "increment")
		return att + E->value ;
	else if (E->name == "set-to")
		return E->value ;
	
}

void oorl_learner :: temp_test1()
{
	
	cout << "Inside temp_test1()" << endl ;
	

	// following test tells if the code is working at all !
	
	FILE * f ;
	f = fopen("test1.txt","w") ;
	for(int i=0;i<P.size();i++)
	{
		fprintf(f,"%s, %s, %s\n",P[i]->a.c_str(), P[i]->att->name.c_str(), P[i]->type.c_str());
		//cout << "Contents :" << endl ;
		for(int j=0;j<P[i]->lstPred.size();j++)
		{
			//cout << 
			fprintf(f,"	%s, %s\n",P[i]->lstPred[j]->model.c_str(), P[i]->lstPred[j]->effect->name.c_str());
		}
		
	}

	fprintf(f,"Failure list Fa\n") ;
	for(int i=0;i<Fa.size();i++)
		fprintf(f,"%s : %s\n",actions[i].c_str(), Fa[i].c_str()) ;

	fclose(f);
	
	
	
}
// for a list of effects for a particular attribute (x) check for all possible pairs 
// return true if they are compatible false if not
bool oorl_learner :: isCompatible(vector<nvpair *> E, int x)
{
	l = E.size() ;
	bool result = true ;
	for(int i=0;i<l;i++)
	{
		for(int j=0;j<l;j++)
		{
			if(i!=j)
			{
				if(E[i]->name == E[j]->name && E[i]->value != E[j]->value) // same effect name but different value? return false
					return false ;
				else if (E[i]->name != E[j]->name) // different name
				{
					if(applyEffect(x, E[i]) != applyEffect(x,E[j])) // and not same effect? return false
						return false ; 
				}
			}
		}
	}
	return true ;
}

// variables moved out 
vector<nvpair *> E ;
int temp_int ;
bool attrFound ;

int oorl_learner :: predictTransition(int s, int a) 
{
	// check in failure states 
	s_cond = cond(s) ;
	attrFound = false;
	//if(itr == 33)
		//cout << "	Inside predictTranslation \n" ; 
	
	if(FailureStateActionCombo(s,a))
		return s ; 
	else
	{
		//// TEMPCODE
		//cout << "Inside else part of predictTransition()" << endl ;
		//initialize the vector details
		for(int j=0;j<nAttr;j++)
			details[j] = state_mat[s][j] ; 
		for(int j=0;j<nAttr;j++)
		{
			E.clear() ;
			temp_int = details[j] ;
			for(int k=0;k<eff_types.size();k++)
			{
				nvp = findMatchingModel(actions[a],attributes[j], eff_types[k], s_cond);
				x = atoi(nvp->name.c_str()) ;
				y = nvp->value ;
				if(y != -1)
				{
					E.push_back(P[x]->lstPred[y]->effect);
				}
			}
			if(E.empty() || !isCompatible(E,temp_int))  
			{
				// TEMPTEST
				//cout << "(s,a) : " << s << "," << a  << " effect list E either empty or not compatible\n" ;
				//return s_max ; // s_max is a pre-defined variable
				continue ; // UPDATE: continue search of exiting here 
			}
			else
			{
				//cout << "control reaches here ..." << endl ;
				// apply all the changes in E to s and get s'
				for(int i=0;i<E.size();i++)
					details[j] = applyEffect(details[j], E[i])  ;
				attrFound= true ;
				break ; // NEW: predictTransition applies change to only one attribute at a time
			} 
		}
		
		//if(itr == 33)
		//	cout << "	Done predictTranslation \n" ;
		
		// if found from above loop, get a 4 length array from details[]
		if(attrFound)
		{
			info[0] = details[0];
			info[1] = details[1] ;
			// depending on details[6] search for appropriate index in locaions[] for passenger and destination.
			if(details[6] == 1) 
				info[3] = D ;
			else
				info[3] = getIndex(Vec2f(details[4],details[5]),world->locations) ;
			info[2] = getIndex(Vec2f(details[2],details[3]),world->locations) ;
			// TEMPCODE
			//for (int i=0;i<4;i++)
			//	cout << details[i] << " " ;
			//cout << endl ; 
			// 
			return getStateNumber(info) ;
		}
		else // no appropriate prediction found, return SMAX
			return s_max ;
	}
		
}

/*given a condition string, and action 'a',see if  any c in Fa(a) is c |= s, if yes and (1) remove all such c and */
/*add s to it */
void oorl_learner :: updateFailure(string s, int a)
{ 
 
	//cout << "updateFailure called with " << s << " and " << a << endl ;
	int l = Fa[a].length() ; 
	int i = 0 ;
	while(i<l)
	{
		if(compareCondStr(s, Fa[a].substr(i,n)))
		{
			//cout << "here" << endl ;
			Fa[a].erase(i,n);
			l = Fa[a].length() ;
		}
		else
			i+= n ;
	}
	Fa[a] = s + Fa[a] ;
	//cout << Fa[a] << endl ;
}





// for each effect type, return the corresponding effect that'll cause the change in attribute natt 
// when going to state s_ from s.

vector<nvpair *> oorl_learner :: eff_att(int natt, int s, int s_)
{
	vector<nvpair *> result ;
	for(int i=0;i<eff_types.size();i++)
	{
		if(eff_types[i] == "increment")
			result.push_back(new nvpair(eff_types[i] , state_mat[s_][natt] -  state_mat[s][natt])) ; 
		else if(eff_types[i] == "set-to")
			result.push_back(new nvpair(eff_types[i] , state_mat[s_][natt])) ; 
	}
	return result ;
}

void oorl_learner :: addExperience(int s, int a, int s_, int k1) 
{
	s_cond = cond(s) ;
	if(s == s_)  // failure , hence update Fa[a]
	{
		// TEMPCODE
		//cout << "calling updateFailure ...\n" ;
		updateFailure(s_cond, a);		
	}
	else
	{
		
		for(int j=0;j<nAttr;j++)
		{
			effectlist = eff_att(j,s,s_);
			// TEMPCODE
			//cout << " effectlist returned : " <<  effectlist.size() << endl ;
			for(int k=0;k<effectlist.size();k++)
			{
				
				nvp = findMatchingModel(actions[a],attributes[j], effectlist[k]->name, effectlist[k]); 
				if(nvp->value != -1) // PREDICTION AVAILABLE
				 {
					 // some p exists
					 x = atoi(nvp->name.c_str()) ;
					 y = nvp->value ; 
					 P[x]->lstPred[y]->model = commutativeOp(P[x]->lstPred[y]->model , s_cond) ;

					 for(int i=0;i<P[x]->lstPred.size();i++)
					 {
						 if(i!=y)
						 {
							 if(compareCondStr(P[x]->lstPred[y]->model , P[x]->lstPred[i]->model))
							 {
								 cout << P[x]->a << "," << P[x]->att->name << "," <<P[x]->type << " deleted due to overlapping conditions after update(effectlist[k])\n" ;
								 P.erase(P.begin()+x) ; // conditions overlap, VIOLATING ASSUMPTIONS
								 break ;
							 }
						 }
					 }  
				 }
				 else  // NO PREDICTION AVAILABLE
				 {
					 nvp = findMatchingModel(actions[a],attributes[j], effectlist[k]->name,s_cond) ;
					 x = atoi(nvp->name.c_str()) ;
					 y = nvp->value ; 
					 if(x!=-1) //  this is an extra if added, since (a,att,type) tripplets are being deleted as well
					 { 
						tbool = false ;
						if(y != -1)				// some c |= cond(s)						 
							tbool = true ; 
						else 					// OR cond(s) |=  some c
						{ 
							for(int i=0;i<P[x]->lstPred.size();i++)
							{
								if(compareCondStr(cond(s),P[x]->lstPred[i]->model))
								{ 
									tbool = true ;
									break ;
								}
							}
						}

						if(tbool)
						{
							 cout << P[x]->a << "," << P[x]->att->name << "," <<P[x]->type << " deleted due to overlapping conditions after update(s_cond)\n" ;
							 P.erase(P.begin()+x) ;  
						}
						else
						{
							// else part of   if( c |= cond(s) or cond(s) |= c  i.e. both the above if and else's failed
							P[x]->lstPred.push_back(new prediction(s_cond,effectlist[k])) ;
							if(P[x]->lstPred.size()>k1)
							{
								cout << P[x]->a << "," << P[x]->att->name << "," <<P[x]->type << " deleted due to excessive k1\n" ;
								P.erase(P.begin()+x) ; 
							}
						}
					 }
				} // end if (count)
			} // end for 
					 
		}// end for
	} // end if
}


void oorl_learner :: saveQ() 
{
	ofstream outStream;
	outStream.open ("policy.txt"); 
	for(  int i=0;i<nstates;i++)
	{
		for(  int j=0;j<nactions;j++)
			outStream << Q[i][j]  << " " ;
		outStream << "\n" ;
	} 
	outStream.close();
	cout << "Q written to policy.txt" << endl ;
}


// load optimal policy during testing mode
void oorl_learner :: loadQ() 
{
	FILE * f ;
	float temp ;
	f = fopen("policy.txt","r") ; 
	s_sim = 97 ;
	for(int i=0;i<nstates;i++)
	{
		for(int j=0;j<nactions;j++)
		{
			fscanf(f, "%f", &temp) ;
			if(temp == 0)
				Q[i][j]  = MIN_VALUE ; // unexplored state, give least importance
			else
				Q[i][j] = temp ;
		}
	} 
	fclose(f);
}


void oorl_learner :: runController() 
{ 
	// decode current state
	cout << "Current State: " << s_sim << endl ;
	for(int i=0;i<7;i++)
		q[i] = state_mat[s_sim][i] ;
	// Draw the current World
	world->Update(q) ;	
	// look up Q to find best a
	a_sim  = getBestAction(s_sim) ;
	// update s for next drawing
	s_sim =  carryOutAction(s_sim,a_sim) ;
	sleep(1) ;  	
}