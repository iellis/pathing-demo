/* File: Demo.cpp
        Ike Ellis, email: icellis@ukans.edu
        KUID 644578, EECS 672, Fall 1999 */


#include "Demo.h"

static const unsigned char      ESC = 27;

Demo* Demo::sCurrent = NULL;
float g_fClimbingPenalty = 1.0f;

/////////////////////////////////////////////////////

Demo::Demo( int WinW, int WinH ) : m_apWorld( new Environment( WORLD_X, 1, WORLD_Z, CLEAR_WORLD ) )
{
	// remember the current object. Here we just set it to the last
	// one created.

	Demo::sCurrent = this;	 
	//initialize timing and demo params
	m_iLastRefresh = timeGetTime();
	m_iLastUpdate = timeGetTime();
	m_bPaused = false;
	m_bShowSearch = true;
	m_bAddMode = false;
	m_bEditMode = false;
	m_bMarkerMode = false;

	//set initial eye point
	m_fCamRadius = 60.0f;
	m_fCurAngle = 0.0f;
	m_tLookAt.x = WORLD_X * 0.5f;
	m_tLookAt.y = 5.0f;
	m_tLookAt.z = WORLD_Z * 0.5f;
	m_fCamPosX = m_fCamRadius *	(float)sin( m_fCurAngle ) + m_tLookAt.x;
	m_fCamPosZ = m_fCamRadius * (float)cos( m_fCurAngle ) + m_tLookAt.z;
	m_fCamPosY = 30.0;	
	m_fCamUp   = 1.0;
	
	m_bShowCtrlPts = false;
	m_bShowMesh = false;
	
	m_bSelectRect = false;
	m_iSelectRectX = 0;
	m_iSelectRectY = 0;

	//set all lights on
	m_iLight0 = 1;
	m_iLight1 = 1;
	

	//init light parameters

	//light 0
	diffuse0[0]=0.4;  diffuse0[1]=0.4;   diffuse0[2]=0.4;  diffuse0[3]=1.0;
	ambient0[0]=0.0;  ambient0[1]=0.0;   ambient0[2]=0.0;  ambient0[3]=0.0;
	specular0[0]=1.0; specular0[1]=0.0;  specular0[2]=0.0; specular0[3]=1.0;
	position0[0]= -WORLD_X ; position0[1]=150.0; position0[2]= -WORLD_Z ; position0[3]=0.0;

	//light 1
//	diffuse1[0]=0.0;  diffuse1[1]=0.1;    diffuse1[2]=0.4;  diffuse1[3]=0.2;
//	ambient1[0]=0.0;  ambient1[1]=0.0;    ambient1[2]=0.0;  ambient1[3]=0.0;
//	specular1[0]=0.0; specular1[1]=0.0;   specular1[2]=1.0; specular1[3]=1.0;
//	position1[0]=0; position1[1]=50.0; position1[2]=WORLD_Z / 2; position1[3]=1.0;

	diffuse2[0]=1.0;   diffuse2[1]=1.0;    diffuse2[2]=1.0;   diffuse2[3]=0.0;
	direction2[0]=0.0; direction2[1]=-1.0; direction2[2]=0.0; direction2[3]=0.0;
	specular2[0]=1.0;  specular2[1]=1.0;  specular2[2]=1.0;  specular2[3]=1.0;
	position2[0]=0.0;  position2[1]=0.0;  position2[2]=0.0;  position2[3]=1.0;
	angle2 = 30.0;
	exponent2 = 100.0;

	

}

/////////////////////////////////////////////////////

Demo::~Demo()
{
 
}

/////////////////////////////////////////////////////
//static callback functions just call instance method
//of the same name
/////////////////////////////////////////////////////
void Demo::DisplayCB()
{  
	//render in normal mode
    Demo::sCurrent->Display(GL_RENDER);
}

/////////////////////////////////////////////////////

void Demo::KeyboardCB(unsigned char key, int x , int y)
{
	Demo::sCurrent->Keyboard(key, x , y);
}

/////////////////////////////////////////////////////

void Demo::MouseFuncCB(int button, int state, int x, int y)
{
	Demo::sCurrent->MouseFunc(button, state, x, y);
}


/////////////////////////////////////////////////////
void Demo::ReshapeCB(int w, int h)
{
	Demo::sCurrent->Reshape( w, h);
}

/////////////////////////////////////////////////////

void Demo::IdleCB()
{
	Demo::sCurrent->Idle();
}

/////////////////////////////////////////////////////

void Demo::MotionCB(int x, int y)
{
	Demo::sCurrent->Motion( x, y);	
}


/////////////////////////////////////////////////////

void Demo::TimerCB( int id )
{
}


/////////////////////////////////////////////////////
//static menu functions initialize glut menus
/////////////////////////////////////////////////////
void Demo::MainMenu(int id)
{
	switch(id)
	{
	//quit
	case 1:
		exit(0);
	break;
	
	//add agent
	case 2:
		Demo::sCurrent->AddAgentMode();
		Demo::sCurrent->m_bEditMode = false;
	break;
	
	//pause
	case 3:
		if(Demo::sCurrent->m_bPaused)
			Demo::sCurrent->m_bPaused = false;
		else
			Demo::sCurrent->m_bPaused = true;
	break;

	//reset
	case 4:
		Demo::sCurrent->m_apWorld = auto_ptr<Environment>( new Environment( WORLD_X, WORLD_Y, WORLD_Z, RANDOM_WORLD ));
		glDisable(GL_LIGHT1);
	break;

	
	case 5:
		if(Demo::sCurrent->m_bShowCtrlPts)
		{
			Demo::sCurrent->m_bShowCtrlPts = false;
			Demo::sCurrent->m_bShowMesh = false;
		}
		else
			Demo::sCurrent->m_bShowCtrlPts = true;
	break;
	/*
	case 6:
		if(Demo::sCurrent->m_bEditMode)
			Demo::sCurrent->m_bEditMode = false;			
		else
		{
			Demo::sCurrent->m_bEditMode = true;
			Demo::sCurrent->m_bAddMode = false;
		}
	break;
*/
	
	case 7:
		if(Demo::sCurrent->m_bShowMesh)
			Demo::sCurrent->m_bShowMesh = false;
		else
			Demo::sCurrent->m_bShowMesh = true;

	break;
	case 8:
		if(Demo::sCurrent->m_bMarkerMode)
			Demo::sCurrent->m_bMarkerMode = false;			
		else
		{
			Demo::sCurrent->m_bMarkerMode = true;
			Demo::sCurrent->m_bAddMode = false;
		}
	break;
	default:
		break;


	}
}

/////////////////////////////////////////////////////

void Demo::SearchMenu(int id)
{
	Demo::sCurrent->m_apWorld->SetAgentSearchStrategy( (e_MoveStrategy)id );

}

void Demo::ClimbingMenu(int id)
{
	switch(id)
	{

	case 1:
		g_fClimbingPenalty = 0.2;
		break;
	
	case 2:
		g_fClimbingPenalty = 0.4;
		break;

	case 3:
		g_fClimbingPenalty = 0.6;
		break;

	case 4:
		g_fClimbingPenalty = 0.8;
		break;

	case 5:
		g_fClimbingPenalty = 1.0;
		break;

	case 6:
		g_fClimbingPenalty = 1.5;
		break;

	case 7:
		g_fClimbingPenalty = 2.0;
		break;

	case 8:
		g_fClimbingPenalty = 4.0;
		break;

	case 9:
		g_fClimbingPenalty = 6.0;
		break;

	case 10:
		g_fClimbingPenalty = 8.0;
		break;
	}

}

/////////////////////////////////////////////////////

void Demo::AgentMenu(int id)
{
	Demo::sCurrent->m_apWorld->SetCurAgent(id);

}

/////////////////////////////////////////////////////

void Demo::TexMapMenu(int id)
{

}


/////////////////////////////////////////////////////
void Demo::Display(GLenum eMode)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	gluLookAt( m_fCamPosX, m_fCamPosY, m_fCamPosZ, m_tLookAt.x, m_tLookAt.y, 
			   m_tLookAt.z, 0.0, m_fCamUp, 0.0 );

	//main lighting
	glLightfv( GL_LIGHT0, GL_AMBIENT, ambient0 );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse0 );
	glLightfv( GL_LIGHT0, GL_SPECULAR, specular0 );
	glLightfv( GL_LIGHT0, GL_POSITION, position0 );

//	glLightfv( GL_LIGHT1, GL_AMBIENT, ambient1 );
//	glLightfv( GL_LIGHT1, GL_DIFFUSE, diffuse1 );
//	glLightfv( GL_LIGHT1, GL_SPECULAR, specular1 );
//	glLightfv( GL_LIGHT1, GL_POSITION, position1 );

	glLightfv( GL_LIGHT1, GL_SPOT_DIRECTION, direction2 );
	glLightf ( GL_LIGHT1, GL_SPOT_EXPONENT, exponent2 );
	glLightf ( GL_LIGHT1, GL_SPOT_CUTOFF, angle2 );
	glLightfv( GL_LIGHT1, GL_DIFFUSE, diffuse2 );
	glLightfv( GL_LIGHT1, GL_SPECULAR, specular2 );
	//glLightfv( GL_LIGHT1, GL_POSITION, position1 );

	m_apWorld->Display(eMode, m_iLastRefresh, m_bShowSearch, m_bPaused, m_bShowCtrlPts, m_bShowMesh);

	glFlush();
	glutSwapBuffers();
}

/////////////////////////////////////////////////////

void Demo::Keyboard(unsigned char key, int x , int y)
{
	//char buf[10];
	//cout << key;
	//OutputDebugString(itoa(key, buf, 10));

  switch (key)
  {

	  //4 key- rotate 10 deg left
	  case 52:
		m_fCurAngle -= (float)TEN_DEG;
		m_fCamPosX = m_fCamRadius *	(float)sin( m_fCurAngle ) + m_tLookAt.x;
		m_fCamPosZ = m_fCamRadius * (float)cos( m_fCurAngle ) + m_tLookAt.z;
	  break;

	  //6 key- rotate 10 deg right
	  case 54:
		m_fCurAngle += (float)TEN_DEG;
		m_fCamPosX = m_fCamRadius *	(float)sin( m_fCurAngle ) + m_tLookAt.x;
		m_fCamPosZ = m_fCamRadius * (float)cos( m_fCurAngle ) + m_tLookAt.z;
	  break;

	  //8 key- moves camera forward
	  case 56:
		if( m_fCamRadius > 7.0 )
		{
			m_fCamRadius -= 2.0;		
			m_fCamPosX = m_fCamRadius *	(float)sin( m_fCurAngle ) + m_tLookAt.x;
			m_fCamPosZ = m_fCamRadius * (float)cos( m_fCurAngle ) + m_tLookAt.z;
		}
	  break;

	  //2 key -moves camera back
	  case 50:
		if(m_fCamRadius < (float)WORLD_X * 1.8)
		{	
			m_fCamPosX = m_fCamRadius *	(float)sin( m_fCurAngle ) + m_tLookAt.x;
			m_fCamPosZ = m_fCamRadius * (float)cos( m_fCurAngle ) + m_tLookAt.z;
			m_fCamRadius += 2.0;			
		}
	  break;

	  //enter- add agent mode
	  case 13:
		AddAgentMode();	
		//Demo::sCurrent->m_bEditMode = false;
		Demo::sCurrent->m_bMarkerMode = false;
	  break;

	  //pause- p key
	  case 112:
		if(Demo::sCurrent->m_bPaused)
			Demo::sCurrent->m_bPaused = false;
		else
			Demo::sCurrent->m_bPaused = true;
	  break;

	  //toggle terrain control point display- c key
	  case 99:
		  if(Demo::sCurrent->m_bShowCtrlPts)
		  {	  Demo::sCurrent->m_bShowCtrlPts = false;
			  Demo::sCurrent->m_bShowMesh = false;
		  }
		  else 
			  Demo::sCurrent->m_bShowCtrlPts = true;

	  break;

	  //show control point mesh- v key
	  case 118:
		  if(Demo::sCurrent->m_bShowMesh)
			  Demo::sCurrent->m_bShowMesh = false;
		  else 
			  Demo::sCurrent->m_bShowMesh = true;

	  break;


	  /*
	  //terrain edit mode- T
	  case 116:
		if(Demo::sCurrent->m_bEditMode)
			Demo::sCurrent->m_bEditMode = false;
		else
		{
			Demo::sCurrent->m_bEditMode = true;
			Demo::sCurrent->m_bAddMode = false;
		}
	  break;
	  */
	  //add marker mode- T
	  case 116:
		if(Demo::sCurrent->m_bMarkerMode)
			Demo::sCurrent->m_bMarkerMode = false;
		else
		{
			Demo::sCurrent->m_bMarkerMode = true;
			Demo::sCurrent->m_bAddMode = false;
		}
	  break;




	  //reset- r key
	  case 114:
	    Demo::sCurrent->m_apWorld = auto_ptr<Environment>( new Environment( WORLD_X, WORLD_Y, WORLD_Z, RANDOM_WORLD ));
		glDisable(GL_LIGHT1);
	  break;

	  case ESC:
      exit(0); // terminate the program when escape key pressed
      break;

  }
}

/////////////////////////////////////////////////////

void Demo::MouseFunc(int button, int state, int xPos, int yPos)
{
	//char out[10];
	// Space for selection buffer
	unsigned int selectBuff[SELECT_BUF_SIZE];

	// Hit counter and viewport storeage
	int hits, viewport[4];


	//if this is a down click, record the position and enable the selection area
	if( state == GLUT_DOWN )
	{
		m_iSelectRectX = xPos;
		m_iSelectRectY = yPos;
		m_bSelectRect = true;
	}

	//if button is released, then process the click position
	if( state == GLUT_UP )
	{
		//disable the selection area
		m_bSelectRect = false;


		// Setup selection buffer
		glSelectBuffer(SELECT_BUF_SIZE, selectBuff);
		
		// Get the viewport
		glGetIntegerv(GL_VIEWPORT, viewport);

		// Switch to projection and save the matrix
		
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();

		// Change render mode
		glRenderMode(GL_SELECT);

		// Establish new clipping volume to be unit cube around
		// mouse cursor point (xPos, yPos) and extending two pixels
		// in the vertical and horzontal direction
		glLoadIdentity();
		gluPickMatrix(xPos, viewport[3] - yPos, 2,2, viewport);

		// Apply perspective matrix 
		gluPerspective((GLfloat)VIEW_ANGLE, m_fAspectRatio, FRUSTUM_NEAR, FRUSTUM_FAR);

		glMatrixMode(GL_MODELVIEW);
		// Draw the scene in select mode
		Display(GL_SELECT);

		
		// Collect the hits
		hits = glRenderMode(GL_RENDER);

		// If a single hit occured, display the info.
		if(hits > 0)
		{
			//sprintf(out, "%i hits!\n", hits);		
			//OutputDebugString(out);
			ProcessMouseHits( selectBuff, hits, button, xPos, yPos );
		}
		// Restore the projection matrix
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		// Go back to modelview for normal rendering
		glMatrixMode(GL_MODELVIEW);
	}		
}

/////////////////////////////////////////////////////

void Demo::ProcessMouseHits( GLuint selectBuff[], int numHits, int button, int winX, int winY )
{
	//char out[20];
	//sprintf(out, "name: %i\n", selectBuff[3]);
	//OutputDebugString(out);

	//default to first hit
	int nearestHit = 0;
	GLuint hitName;
//	double  approxZ;

	//find the hit with the lowest z value
	for(int curHit=1; curHit < numHits; curHit++)
	{
		//compare minimum z values
		if( selectBuff[curHit*4+1] < selectBuff[ (curHit-1) * 4 + 1] )		
			nearestHit = curHit;				
	}

	//store the name of the nearest hit
	hitName = selectBuff[nearestHit*4+3];

	//get the approximate z value
	//approxZ = ( (double)selectBuff[nearestHit*4+1] + (double)selectBuff[nearestHit*4+2] ) * 0.5;
	
	//find the world coordinates of this point
	//TripleFloat pos = GetWorldCoords( (double)winX, (double)m_iCurWH-(double)winY, approxZ );

	//if in add agent mode
	if(m_bAddMode)
	{
		//if it's a board space
		if(hitName > MAX_AGENTS)
		{
			//add an agent

			m_apWorld->AddAgent(hitName-(MAX_AGENTS+1));
		}
		
		//end add agent mode
		AddAgentMode(false);		
	}
	else if(m_bMarkerMode)
	{
		//if it's a board space
		if(hitName > MAX_AGENTS)
		{
			//add a marker
			m_apWorld->AddMarker( hitName-(MAX_AGENTS+1) );
			m_bMarkerMode = false;
		}
	}
	//if not in add agent mode
	else
	{
		//if it's an agent, 
		if(hitName <= MAX_AGENTS)
		{
			//select the agent
			m_apWorld->SetCurAgent(hitName);
				
		}
		//if it's a board space
		else
		{
			//give orders to the current agent
			m_apWorld->SpaceClicked( hitName-(MAX_AGENTS+1), m_bShowSearch);			
		}
	}

}

/////////////////////////////////////////////////////

void Demo::Reshape(int w, int h)
{	
	// Prevent a divide by zero
	if(h == 0)
		h = 1;

	// Calculate aspect ratio of the window
	m_fAspectRatio = (GLfloat)w/(GLfloat)h;

	//set up viewport and clipping area
	glViewport( 0.0, 0.0, w, h );
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	
	// Field of view of 45 degrees, near and far planes 1.0 and 425
	gluPerspective((GLfloat)VIEW_ANGLE, m_fAspectRatio, FRUSTUM_NEAR, FRUSTUM_FAR);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//clear the background
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable( GL_DEPTH_TEST );
	glFlush();

	//save new window size
	m_iCurWW = w;
	m_iCurWH = h;

	glutPostRedisplay();
}

/////////////////////////////////////////////////////

void Demo::Idle()
{
	//char out[15];

	//get the current time
	GLuint curTime = timeGetTime();
	GLfloat time = 1.0/(curTime - m_iLastRefresh)*1000;

	
	//if it's time for an update
	if( ((curTime - m_iLastUpdate) > (GLuint)UPDATE_PERIOD) && !m_bPaused )
	{
		m_apWorld->Update();
		m_iLastUpdate = curTime;
	}
	//if it's time for a refresh
	if( (curTime - m_iLastRefresh) > (GLuint)REFRESH_PERIOD )
	{		
		//sprintf(out, "%f\n", time );
		//OutputDebugString(out ); 

		glutPostRedisplay();
		m_iLastRefresh = curTime;				
	}

}


/////////////////////////////////////////////////////

void Demo::Motion(int x, int y)
{
	glColor3f(0,1,0);
	glBegin(GL_LINE_LOOP);
		glVertex2d(m_iSelectRectX, m_iSelectRectY);
		glVertex2d(m_iSelectRectX, (float)y);
		glVertex2d((float)x, m_iSelectRectY);
		glVertex2d((float)x, (float)y);
	glEnd();	
}


/////////////////////////////////////////////////////

void Demo::Timer( int id )
{
	
}

/////////////////////////////////////////////////////

void Demo::AddAgentMode( bool bMode )
{
	//set cursor
	//if(bMode)
	//	SetCursor(LoadCursor(NULL, IDC_IBEAM));
	//else
	//	SetCursor(LoadCursor(NULL, IDC_ARROW));

	//set modes
	m_bAddMode = bMode;
	m_bPaused = bMode;
}

/////////////////////////////////////////////////////

//find the world coordinates of a screen click
TripleFloat Demo::GetWorldCoords( double winX, double winY, double approxZ )
{
	TripleFloat coords;
	double modelView[16];
	double projection[16];
	int viewPort[4];
	double wCoords[3];

	glGetDoublev( GL_MODELVIEW_MATRIX, modelView );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewPort );
	
	//find the worldspace coords of the screen click
	gluUnProject( winX, winY, approxZ, 
				  modelView, projection, viewPort, 
				  &wCoords[0], &wCoords[1], &wCoords[2]);

	coords.x = (float)wCoords[0];
	coords.y = (float)wCoords[1];
	coords.z = (float)wCoords[2];

	return coords;

}
