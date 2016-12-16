	all code snippets go in the "mouse_func"  function in controller.cpp
	
	
	
	//TESTING compare condStr / commutativeOp
		//if(sprev[0] != ' ')
		//	cout << m_learn->commutativeOp(snew,sprev) << endl  ; 
		//sprev = snew ;
		// testing ends	
		
		
------------------------------------------------------------------------------------------------------		
		
		// test updateFailure
		m_learn->DOORMAX_main();
		m_learn->updateFailure("1010100",2);
		// test ends 
		
		and uncomment changes in updateFailure()
-------------------------------------------------------------------------------------------------------------------
		
		
	// write the initiated states to file (DONE)
	
	FILE * f ;
	f = fopen("check_states.txt","w") ;
	for(int i=0;i<nstates;i++)
	{
		for(int j=0;j<nAttr;j++)
			fprintf(f,"%d ",state_mat[i][j]);
		fprintf(f,"\n") ;
	}
	fclose(f);
	
------------------------------------------------------------------------------------------------	
// testing code in temp_test1()
	
	
// test findMatchingModel
cout << "Testing findMatchingModel()...." << endl ;

P[10]->lstPred.push_back(new prediction("1001011",new nvpair("increment",5))) ; // test data added
P[10]->lstPred.push_back(new prediction("1011011",new nvpair("increment",3))) ; // test data added

cout << P[10] ->a << " (" << P[10] ->att->name << " " << P[10]->att->value << ") " <<   P[10] -> type <<" "  <<P[10]->lstPred[0]->model << endl ;

//nvpair * result = findMatchingModel("north", new nvpair("passg_y",5),"increment","10*1011") ;  // <-- test 1st type 
nvpair * result = findMatchingModel("north", new nvpair("passg_y",5),"increment",new nvpair("increment",3)) ;  // <-- test second type
cout << result->name << " , " << result->value << endl ; 

// testing eff_att , for correspondence also look into check_states.txt	
vector<nvpair *> result = eff_att(5,5,7) ;
cout << result[0]->name << " " << result[0]->value << endl ;
cout << result[1]->name << " " << result[1]->value << endl ;

// testing commutativeOp
cout << "Testing commutativeOp()" << endl ;
cout << commutativeOp("1010010","0011010") << endl  ;

// testing updateFailure()
Fa[2] = "1001001000000110101000000001" ;
updateFailure("0000001",3) ;
cout << Fa[3] << endl ;	


//cout << "Testing FailureStateActionCombo() .." << endl ;
string s = cond(20) ;
cout << s << endl ;
Fa[4] = "11100010000000" + s ; 
cout << FailureStateActionCombo(20,4) << endl ;


//cout << "Testng isCompatible ... " << endl  ;
vector<nvpair *> e ;
e.push_back(new nvpair("increment",3)) ;
e.push_back(new nvpair("set-to",6)) ;  // ("set-to",8) works because x =5 , look in isCompatible()ss
cout << isCompatible(e,4) << endl ;

// testing carryOutAction()

int s = 0;
int a ;
int s1 ;
while(s != -1)
{
	cout << "enter state and action \n"  ;
	cin >> s >> a ;
	cout << s << " " << state_mat[s][0] << " " << state_mat[s][1] << endl ;
	s1 = carryOutAction(s,a) ;
	cout << s1 << " " << state_mat[s1][0] << " "<< state_mat[s1][1]<< endl ;
	 
}
