
#include "Environment.h"


///////////////////////////////////////////////////////////////////////////////////

Environment::Environment( int XSize, int YSize, int ZSize, enum e_EnvType EnvType ) : 
	m_apTerrain( new BezierSurface( SURFACE_U_ORDER, SURFACE_V_ORDER, XSize, ZSize, 
									TESSELATE_DEGREE_Z, TESSELATE_DEGREE_X, MAX_TERRAIN_ELEVATION ) )
{

	m_iXSize = XSize;
	m_iYSize = YSize;
	m_iZSize = ZSize;

	m_iXNodes = TESSELATE_DEGREE_X;
	m_iZNodes = TESSELATE_DEGREE_Z;

	long t;
	srand( time(&t) );

	
	//set the clicked space to an invalid index

	m_iCurAgent = -1;

//	//generate a world
//	switch( EnvType )
//	{

//	case CLEAR_WORLD:
		GenClearWorld();		
//		break;

//	case RANDOM_WORLD:
//		GenRandWorld();		
//		break;

//	}	

	agentRed[0] = .98; agentGreen[0] = 1; agentBlue[0] = .066;
	agentRed[1] = 153.0 / 255.0; agentGreen[1] = 236.0 / 255.0; agentBlue[1] = 255.0 / 255.0;
	agentRed[2] = 204.0 / 255.0; agentGreen[2] = 0.0 / 255.0; agentBlue[2] = 15.0 / 255.0;
	agentRed[3] = 0.0 / 255.0; agentGreen[3] = 204.0 / 255.0; agentBlue[3] = 10.0 / 255.0;
	agentRed[4] = 238.0 / 255.0; agentGreen[4] = 0.0 / 255.0; agentBlue[4] = 221.0 / 255.0;
	agentRed[5] = 255.0 / 255.0; agentGreen[5] = 153.0 / 255.0; agentBlue[5] = 156.0 / 255.0;
	agentRed[6] = 170.0 / 255.0; agentGreen[6] = 138.0 / 255.0; agentBlue[6] = 0.0 / 255.0;
	//agentRed[7] = 250; agentGreen[7] = 255; agentBlue[7] = 17;
	//agentRed[8] = 250; agentGreen[8] = 255; agentBlue[8] = 17;
	//agentRed[9] = 250; agentGreen[9] = 255; agentBlue[9] = 17;
	//agentRed[10] = 250; agentGreen[10] = 255; agentBlue[10] = 17;
	//agentRed[11] = 250; agentGreen[11] = 255; agentBlue[11] = 17;
	//agentRed[12] = 250; agentGreen[12] = 255; agentBlue[12] = 17;


	//set up the agent colors
	for(int i = 7; i < MAX_AGENTS; i++)
	{
		if( i & 1 )
		{
			agentRed[i] = 0.1 + .15 * i;
			agentBlue[i] = 0;
		}
		else
		{
			agentRed[i] = 0;
			agentBlue[i] = 0.1 + .15 * i;
		}

		agentGreen[i] = 0.5;
	}

	InitDisplayLists();
}

///////////////////////////////////////////////////////////////////////////////////
void Environment::Update()
{	
	//move all the agents
	vector<Agent>::iterator agent;
	for( agent = m_vAgentList.begin(); agent != m_vAgentList.end(); ++agent )
		agent->Move(m_vWorldState, *m_apTerrain);

}
///////////////////////////////////////////////////////////////////////////////////
void Environment::Display( GLenum eMode, GLuint lastUpdate, bool showSearch, bool bPaused, bool bShowCtrlPts, bool bShowMesh )
{
	
//	GLuint curData;	
	GLuint curTime = timeGetTime();
	int agentPos;
	int agentNum;
	TripleFloat vertPos;		
	float lightPos[4];
	static float rotAngle = 0.0f;

	//spin the goal angle 10 deg per frame
	rotAngle += 10.0;

	//put the spotlight on the current agent
	if( !m_vAgentList.empty() )
	{
		agentPos = m_vAgentList[m_iCurAgent-1].GetCurPos();
		vertPos = m_apTerrain->GetVertPos(agentPos);
		lightPos[0] = vertPos.x;
		lightPos[1] = vertPos.y + 5.0f;
		lightPos[2] = vertPos.z;
		lightPos[3] = 1.0f;

		glLightfv( GL_LIGHT1, GL_POSITION, lightPos );

	}

	//if any agents are searching, let them expand another set of nodes
	if(!m_vAgentList.empty() )
	{
		vector<Agent>::iterator cur;
		for(cur = m_vAgentList.begin(); cur != m_vAgentList.end(); ++cur)
		{
			if(cur->Searching() )
			{		

				//expand the next set of nodes
				if(!bPaused)
					cur->PlanPath(m_vWorldState, *m_apTerrain, showSearch);
				
				
				//search space nodes don't count if clicked on, so don't draw them									
				//display the current search space if enabled
				if( showSearch && eMode == GL_RENDER)
				{									
					TripleFloat loc;	
					list<NodeType> &searchSpace = cur->GetSearchSpace();
					
					list<NodeType>::iterator nodes;			
					if( !searchSpace.empty() )
					{					
						agentNum = cur->GetAgentNum() - 1;
						for(nodes = searchSpace.begin(); nodes != searchSpace.end(); ++nodes)
						{			
									
							//get the position of the node
							loc = m_apTerrain->GetVertPos( nodes->Position );
								
							//draw a marker
							glPushMatrix();
								glTranslatef( loc.x, loc.y, loc.z );
								glColor3f( agentRed[agentNum], agentGreen[agentNum], agentBlue[agentNum] );
								glCallList(m_iSearchNodeDL);
							glPopMatrix();

							//draw the goal of this agent
							int goalPos = cur->GetGoal();
							loc = m_apTerrain->GetVertPos( goalPos );
							glPushMatrix();
								//glLoadIdentity();								
								glTranslatef( loc.x, loc.y+1.0, loc.z );
								glRotatef(rotAngle, 0.0, 1.0, 0.0);															
								glCallList(m_iSelectSpaceDL);
							glPopMatrix();
							
						}											
						
					}
					
				}	
			}
		}
	}
	
	//if selection is on, init the name stack
	if( eMode == GL_SELECT )
	{
		glInitNames();
		glPushName(0);
	}

	//display the terrain
	m_apTerrain->Display( bShowCtrlPts, bShowMesh, eMode );

	//display the agents
	vector<Agent>::iterator curAgent;
	int agent = 0;

	//for each agent
	for(curAgent = m_vAgentList.begin(); curAgent != m_vAgentList.end(); ++curAgent)
	{
		//get it's actual position
		agentPos = curAgent->GetCurPos();
		vertPos = m_apTerrain->GetVertPos(agentPos);
		
		//translate 
		glPushMatrix();
		glTranslatef(vertPos.x, vertPos.y, vertPos.z);

		//set the color
		glColor3f(agentRed[agent], agentGreen[agent], agentBlue[agent]);

		if( eMode == GL_SELECT )
		{
			glLoadName( agent+1 );

		}

		//draw
		glCallList(m_iAgentDL);
		glPopMatrix();

		//get the agent's search space
		list<int> &path = curAgent->GetPath();

		TripleFloat loc;
		//draw the path that the agent is following
		list<int>::iterator curNode;
		for(curNode = path.begin(); curNode != path.end(); ++curNode)
		{
			//get the position of the node
			loc = m_apTerrain->GetVertPos( *curNode );
								
			//draw a marker
			glPushMatrix();
				glTranslatef( loc.x, loc.y, loc.z );				
				glCallList(m_iSearchNodeDL);
			glPopMatrix();
		}


		agent++;
	}

	//draw any terrain markers
	list<int>::iterator mark;
	TripleFloat loc;
	for(mark = m_lMarkerList.begin(); mark != m_lMarkerList.end(); ++mark)
	{
		loc = m_apTerrain->GetVertPos( *mark );
		if( eMode == GL_SELECT )		
				glLoadName( MAX_AGENTS+1+(*mark) );

		glColor3f(1.0,0,0);
		glPushMatrix();
			glTranslatef( loc.x, loc.y, loc.z );
			glCallList(m_iBlockedSpaceDL);
		glPopMatrix();

	}				

}


///////////////////////////////////////////////////////////////////////////////////

void Environment::AddAgent( int Pos )
{
	
	int len = m_vAgentList.size();
	
	//if this is the first agent, enable the spotlight
	if(m_vAgentList.empty())
		glEnable(GL_LIGHT1);


	//if more allowed
	if(len < MAX_AGENTS)
	{
		//if the selected space is open
		if( m_vWorldState[Pos] == CLEAR_SPACE )
		{
			m_vAgentList.push_back( *(new Agent( Pos, len+1, m_iXNodes, m_iYSize, m_iZNodes )) );	
			m_vWorldState[Pos] = (unsigned char)m_vAgentList.size();

			//set the new agent selected
			m_iCurAgent = len + 1;
		}
	}

	
}

///////////////////////////////////////////////////////////////////////////////////

void Environment::SetAgentGoal( int AgentNum, int Pos )
{

}

///////////////////////////////////////////////////////////////////////////////////

void Environment::SetAgentSearchStrategy( e_MoveStrategy moveStrat )
{
	if(!m_vAgentList.empty())
		(m_vAgentList.at(m_iCurAgent-1)).SetMoveStrategy( moveStrat );
}

///////////////////////////////////////////////////////////////////////////////////

void Environment::SpaceClicked( int selectSpace, bool showSearch )
{
	if(!m_vAgentList.empty() &&
		m_vWorldState.at(selectSpace) != BLOCKED_SPACE )
	{
		//windows specific...kill it for other platforms
		//load hourglass cursor
		//SetCursor( LoadCursor(NULL, IDC_WAIT) );

		(m_vAgentList.at(m_iCurAgent-1)).SetGoal(selectSpace);
		(m_vAgentList.at(m_iCurAgent-1)).PlanPath(m_vWorldState, *m_apTerrain, showSearch);

		//return to normal cursor
		//SetCursor( LoadCursor(NULL, IDC_ARROW) );
	}

}

///////////////////////////////////////////////////////////////////////////////////

void Environment::AddMarker( int location )
{
	m_lMarkerList.push_back( location );
}

///////////////////////////////////////////////////////////////////////////////////

void Environment::SetCurAgent(int curAgent)
{
	if(curAgent > 0 && curAgent <= m_vAgentList.size() )
		m_iCurAgent = curAgent;
}


///////////////////////////////////////////////////////////////////////////////////

void Environment::EditSpace(int space)
{
	if(m_vWorldState.at(space) == CLEAR_SPACE)
		m_vWorldState.at(space) = BLOCKED_SPACE;
	else
		m_vWorldState.at(space) = CLEAR_SPACE;

}

///////////////////////////////////////////////////////////////////////////////////

void Environment::InitDisplayLists()
{
	//get unique id's
	int ListIDBase = glGenLists(5);
	m_iClearSpaceDL = ListIDBase;
	m_iAgentDL = ListIDBase + 1;
	m_iBlockedSpaceDL = ListIDBase + 2;
	m_iSelectSpaceDL = ListIDBase + 3;
	m_iSearchNodeDL = ListIDBase + 4;
	
	//blank space
	glNewList( m_iClearSpaceDL, GL_COMPILE );
	  glPushMatrix();
	  glBegin( GL_TRIANGLE_STRIP );
	
		glNormal3f( 0, 1, 0 );
			  			  
	    glVertex3f( -0.5, 0, -0.5 );
		glVertex3f( -0.5, 0, 0.5 );
		glVertex3f( 0.5, 0, -0.5 );
		glVertex3f( 0.5, 0, 0.5 );		
	  	  
	  glEnd();
      glPopMatrix();
    glEndList();

	//blocked space
	glNewList( m_iBlockedSpaceDL, GL_COMPILE );
	  glPushMatrix();
	  //glScalef(0.5, 0.5, 0.5);
	  glBegin( GL_TRIANGLE_STRIP );
	
		//top
		glNormal3f( 0, 1, 0 );			  			  
	    glVertex3f( -0.25, .5, -0.25 );
		glVertex3f( -0.25, .5, 0.25 );
		glVertex3f( 0.25, .5, -0.25 );
		glVertex3f( 0.25, .5, 0.25 );		
	  glEnd();
	  glBegin( GL_TRIANGLE_STRIP );
		//bottom
		glNormal3f( 0, -1, 0 );			  			  
	    glVertex3f( -0.25, 0, -0.25 );
		glVertex3f( -0.25, 0, 0.25 );
		glVertex3f( 0.25, 0, -0.25 );
		glVertex3f( 0.25, 0, 0.25 );
	  glEnd();
	  glBegin( GL_TRIANGLE_STRIP );
		//left
		glNormal3f( -1, 0, 0 );
		glVertex3f( -0.25, 0, -0.25 );
		glVertex3f( -0.25, 0, 0.25 );
		glVertex3f( -0.25, .5, -0.25 );
		glVertex3f( -0.25, .5, 0.25 );
	  glEnd();
	  glBegin( GL_TRIANGLE_STRIP );
		//right
		glNormal3f( 1, 0, 0 );
		glVertex3f( 0.25, 0, -0.25 );
		glVertex3f( 0.25, 0, 0.25 );
		glVertex3f( 0.25, .5, -0.25 );
		glVertex3f( 0.25, .5, 0.25 );
	  glEnd();
	  glBegin( GL_TRIANGLE_STRIP );
		//front
		glNormal3f( 0, 0, -1 );
		glVertex3f( -0.25, 0, -0.25 );
		glVertex3f( 0.25, 0, -0.25 );
		glVertex3f( -0.25, .5, -0.25 );
		glVertex3f( 0.25, .5, -0.25 );
	  glEnd();
      glBegin( GL_TRIANGLE_STRIP );
		//back
		glNormal3f( 0, 0, 1 );
		glVertex3f( -0.25, 0, 0.25 );
		glVertex3f( 0.25, 0, 0.25 );
		glVertex3f( -0.25, .5, 0.25 );
		glVertex3f( 0.25, .5, 0.25 );
	  
	  glEnd();
      glPopMatrix();
    glEndList();


	//agent display list
	glNewList(m_iAgentDL, GL_COMPILE);
	  glPushMatrix();
		glTranslatef(0, 0.5, 0);
		glutSolidSphere(0.5, 6, 6);
	  glPopMatrix();
	glEndList();


	
	//selected space 
	glNewList( m_iSelectSpaceDL, GL_COMPILE );
	  glPushMatrix();				
		glutSolidOctahedron();
      glPopMatrix();
    glEndList();


	//search node space
	glNewList( m_iSearchNodeDL, GL_COMPILE );
	  glPushMatrix();	  		
		glutSolidSphere(.3, 6, 6);
      glPopMatrix();
    glEndList();

    //marker 
	

}


//////////////////////////////////////////////////

void Environment::GenClearWorld()
{
	int wsize = m_iXNodes * m_iZNodes;

	//init a clear world		
	for(int i = 0; i < wsize; i++ )
		m_vWorldState.push_back( CLEAR_SPACE );
}


//////////////////////////////////////////////////
