#ifndef _ENVIRONMENT_H
#define _ENVIRONMENT_H

#include "Agent.h"
#include "BezierSurface.h"
#include <vector>
#include <GL/glut.h>
#include <time.h>
#include <math.h>
#include <windows.h>

using namespace std;

class Environment
{

public:
	
	Environment( int XSize, int YSize, int ZSize, e_EnvType EnvType = RANDOM_WORLD );

	void Update();
	void Display(GLenum eMode, GLuint lastUpdate, bool showSearch, bool bPaused, bool bShowCtrlPts, bool m_bShowMesh );

	void AddAgent( int pos );
	void SetAgentGoal( int AgentNum, int Pos );
	void SetAgentSearchStrategy( e_MoveStrategy moveStrat );	

	void SpaceClicked( int selectSpace, bool showSearch );
	void AddMarker( int location ); 


	void SetCurAgent(int curAgent);	
	
	void EditSpace(int space);

	//agent colors
	GLfloat   agentRed[MAX_AGENTS];   
	GLfloat   agentGreen[MAX_AGENTS]; 
	GLfloat   agentBlue[MAX_AGENTS];  

private:

	

	//simulation parameters
	vector<unsigned char> m_vWorldState;				//current state includes all agents and map elements									
	vector<Agent> m_vAgentList;	//list of agents

	auto_ptr<BezierSurface> m_apTerrain;//pointer to terrain bezier surface

	list<int>m_lMarkerList;				//list of markers placed on the board

	int m_iXSize;						//X size of env
	int m_iYSize;						//Y size of env
	int m_iZSize;						//Z size of env

	int m_iXNodes;						//# of nodes in x direction
	int m_iZNodes;						//# of nodes in z direction
			
	int	m_iCurAgent;					//currently selected agent

	//internal procedures
	void InitDisplayLists();			//Sets up the display lists for the various objects
	void GenRandWorld();				//sets up a random world
	void GenClearWorld();				//sets up a clear world
		

	//graphics parameters
	int m_iClearSpaceDL;				//blank space display list
	int m_iBlockedSpaceDL;				//Obstacle display list
	int m_iAgentDL;						//agent display list
	int m_iSelectSpaceDL;				//selected space display list
	int m_iSearchNodeDL;				//search node display list

};

#endif