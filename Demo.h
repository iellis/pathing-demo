/* File: Demo.h
        Ike Ellis, email: icellis@ukans.edu
        KUID 644578, EECS 672, Fall 1999 */

#ifndef _DEMO_H
#define _DEMO_H


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <memory>
#include <iostream.h>
#include "Environment.h"


//menu callback enumeration
enum MenuEntries
{
  LIGHT_ZERO=0,
  LIGHT_ONE=1,
  LIGHT_TWO=2
};



class Demo
{
	public:
		Demo(int WinW, int WinH);
		~Demo();

		// class methods for OpenGL event callbacks. 
		static void DisplayCB();
		static void KeyboardCB(unsigned char key, int x, int y);
		static void MouseFuncCB(int button, int state, int x, int y);
		static void ReshapeCB(int w, int h);
		static void IdleCB();
		static void MotionCB( int x, int y );
		static void TimerCB( int id );
		static void MainMenu(int id);
		static void SearchMenu(int id);
		static void ClimbingMenu(int id);
		static void AgentMenu(int id);
		static void TexMapMenu(int id);
  
	
  
	private:
		// class variables.
		static Demo*  sCurrent;
  	
		// instance methods for the class
		void    Display(GLenum eMode);
		void	Keyboard(unsigned char key, int x, int y);
		void	MouseFunc(int button, int state, int x, int y);
		void	ProcessMouseHits(GLuint selectBuff[], int numHits, int button, int winX, int winY );
		void    Reshape( int w, int h );
		void	Idle();
		void	Motion( int x, int y );
		void	Timer( int id );		
		void	AddAgentMode(bool bMode = true);
		//find the world coordinates of a screen click
		TripleFloat GetWorldCoords( double winX, double winY, double approxZ );

		//parameters for window
		int         m_iCurWH;		//current window height
		int         m_iCurWW;		//current window width
		GLfloat		m_fAspectRatio; //current window aspect ratio
				
		//current eye point
		GLfloat		m_fCamPosX;
		GLfloat		m_fCamPosY;
		GLfloat		m_fCamPosZ;
		GLfloat		m_fCamRadius;
		GLfloat		m_fCamUp;
		GLfloat		m_fCurAngle;
		TripleFloat	m_tLookAt; //center of the environment

		
		//timing variables	
		GLuint		m_iLastRefresh;	//time of last frame update
		GLuint		m_iLastUpdate;	//time of last sim update
		bool		m_bPaused;		//is updating paused
		
		//display parameters
		bool m_bShowCtrlPts;
		bool m_bShowMesh;

		bool m_bSelectRect;
		float  m_iSelectRectX;
		float  m_iSelectRectY;

		//Demo parameters
		std::auto_ptr <Environment>	m_apWorld; //pointer to the world representation

		bool		m_bShowSearch;  //should the search process be shown
		bool		m_bAddMode;		//is the demo currently in "add agent" mode
		bool		m_bMarkerMode;  //add terrain AddAgentMode
		bool		m_bEditMode;    //the demo is in terrain edit mode

		//int m_iSelected;		    //space selected by cursor
			
		//light parameters
		int 	    m_iLight0;		//are the lights on?
		int		    m_iLight1;		
		int		    m_iLight2;

	
		GLfloat	    diffuse0[4];
		GLfloat	    ambient0[4];
		GLfloat	    specular0[4];
		GLfloat     position0[4];

		GLfloat	    diffuse2[4];
		GLfloat	    ambient2[4];
		GLfloat	    specular2[4];
		GLfloat     position2[4];
		GLfloat		direction2[4];
		GLfloat		exponent2;
		GLfloat     angle2;



};

#endif

