// TO DO stuff 
// - add init_learning() function in the else part of isLearning mode 

#include "oorl_learner.h"

headController * mainControl ;
oorl_learner * m_learn ;
bool isLearningMode ;
int queryState ;

int int_glOrtho ; 
// temp part testing
string snew ;
string sprev = " " ; 
// temp part ends

int qnew[7] ;

void Update()
{
	if(queryState == -1)
		m_learn->runController() ;
	else
		mainControl->Update(qnew) ;
	 
}

void mouse_func(int button, int state, int x, int y)
{
	// temporary part added to check the cond function
	int X,Y ; 
	if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		//cout << x  << " " << y << "\n" ;
		X= 2*(x - win_x/2) ; 
		Y = 2*(win_y/2 - y); 
		//set agent position to this and call cond()
		mainControl->taxi->m_pos = Vec2f(X,Y) ;
		//intKey=intKey?0:1 ;
		snew  = m_learn->cond(m_learn->getStateFromWorld()) ;
		cout << snew << endl ;  
	}
}

void OnSize(int width, int height)
{
	glViewport(0,0,width,height);
}

void pre_display()					// keep this in this file with the main function, 
{
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//int_glOrtho = win_x/100 ;
	//gluOrtho2D(-int_glOrtho,int_glOrtho,-int_glOrtho,int_glOrtho);
	gluOrtho2D(-win_x,win_x,-win_y,win_y);
	glViewport(0,0,win_x,win_y);
	
}


void DrawAxes()
{
	glColor3f(0.0,0.0,0.0); 
	
	for(int j=-win_y;j<win_y;j+= 2*gridSize)  // change this to 2x gtile_x
	  {
	  	glBegin(GL_LINES) ;
	  	glVertex2f(-win_x,j);
	  	glVertex2f(win_x,j);
	  	glEnd(); 
	  }
	  for(int j=-win_x;j<win_x;j+= 2*gridSize)
	  {
	  	glBegin(GL_LINES) ;
	  	glVertex2f(j,-win_y);
	  	glVertex2f(j,win_y);
	  	glEnd();
		
	  } 
}

void Draw() // their will be two versions of this function !
{
	
	glClearColor(0.4,0.2,0.1,0.5f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;  
	//glColor3f(0.2,0.2,0.2); 
	DrawAxes();
	m_learn->world->DrawObjects(); // mainControl->DrawObjects(); should work, check NEXT !!!
	glutSwapBuffers();	  
}

void open_glut_window ( void )
{
	//int win_id;
	glutInitDisplayMode ( GLUT_RGBA | GLUT_DOUBLE );
	glutInitWindowPosition ( 0, 0 );
	glutInitWindowSize ( win_x, win_y );
	glutCreateWindow ( "OORL" );

	glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear ( GL_COLOR_BUFFER_BIT );
	glutSwapBuffers ();

	pre_display (); 
	// glutKeyboardUpFunc(key_up);
	//glutKeyboardFunc ( key_press );
	glutMouseFunc ( mouse_func );
	//glutMotionFunc ( motion_func );
	//glutReshapeFunc ( reshape_func );
	glutIdleFunc ( Update );
	glutDisplayFunc ( Draw);
}

int main(int argn, char **argv)
{ 
	glutInit(&argn,argv);
	mainControl = new headController() ;
	mainControl->InitSystem() ; 
	m_learn = new oorl_learner(7, 7,mainControl);
	
	if(argn < 3)
	{
		cout << "Usage: oorl_learner <islearningMode?1/0> <numberIterations/QueryStatetoSee>" ;
		exit(1) ;
	}
	else
	{
		isLearningMode = atoi(argv[1]) ;
		if(!isLearningMode)
		{
			queryState = atoi(argv[2]) ;
			if(queryState != -1)
			{ 
				for(int i=0;i<7;i++)
					qnew[i] = m_learn->state_mat[queryState][i] ; 
				cout << m_learn->cond(queryState) << endl ; 
			}
			else
				m_learn->loadQ() ;
			open_glut_window();
			glutMainLoop();
		}
		else 
		{
			m_learn->DOORMAX_main(atoi(argv[2])) ;
			m_learn->saveQ() ;
			//system("rm test1.txt") ;
			m_learn->temp_test1() ;
			//system("scite test1.txt") ;
			//cout << m_learn->getAction(7)  << " , " ;
		}
	}
	return 0 ;
}
 