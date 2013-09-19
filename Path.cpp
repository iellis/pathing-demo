

#include <stdlib.h>
#include <iostream.h>
#include <stdio.h>
#ifdef WIN32
#include <windows.h>
#endif

#include "Demo.h"

static void transferControlToGLUT()
{
  //glutMotionFunc(Demo::MotionCB);
  glutDisplayFunc( Demo::DisplayCB );
  glutKeyboardFunc( Demo::KeyboardCB );
  glutMouseFunc( Demo::MouseFuncCB );
  glutReshapeFunc( Demo::ReshapeCB );
  glutIdleFunc( Demo::IdleCB );  
  glutMainLoop();
}

void InitMenu()
{

  //texture selection menu
  int SearchStratID = glutCreateMenu( Demo::SearchMenu );
  glutAddMenuEntry( "Random", RANDOM );
  glutAddMenuEntry( "Depth First" , DEPTH_FIRST );
  glutAddMenuEntry( "Breadth First", BREADTH_FIRST );
  //glutAddMenuEntry( "Iterative Deepening" , ITERATIVE_DEEPENING );
  glutAddMenuEntry( "Greedy", GREEDY );
  glutAddMenuEntry( "A*", A_STAR );

  int ClimbID = glutCreateMenu( Demo::ClimbingMenu );
  glutAddMenuEntry( "Most Direct", 1);
  glutAddMenuEntry( "2", 2);
  glutAddMenuEntry( "3", 3);
  glutAddMenuEntry( "4", 4);
  glutAddMenuEntry( "Default", 5);
  glutAddMenuEntry( "6", 6);
  glutAddMenuEntry( "7", 7);
  glutAddMenuEntry( "8", 8);
  glutAddMenuEntry( "9", 9);
  glutAddMenuEntry( "Most Optimal", 10);



  //main menu
  glutCreateMenu( Demo::MainMenu );  
  glutAddSubMenu( "Search Strategy...", SearchStratID );
  glutAddSubMenu( "A* Optimality Parameter", ClimbID );
  glutAddMenuEntry( "Add Agent Mode [Enter]", 2 );
  glutAddMenuEntry( "Show Terrain Surface Control Points [C]", 5 );
  glutAddMenuEntry( "Show Control Point Mesh [V]", 7 );
  //glutAddMenuEntry( "Terrain Edit Mode [T]", 6 );
  glutAddMenuEntry( "Add Terrain Marker Mode [T]", 8);
  glutAddMenuEntry( "Pause [P]", 3 );
  glutAddMenuEntry( "Reset World [R]", 4 );
  glutAddMenuEntry( "Move Camera with Numeric Keypad", 5);
  glutAddMenuEntry( "Quit", 1 );
  glutAttachMenu( GLUT_RIGHT_BUTTON );



}

////////////////////////////////////////////////////////////

TripleInt LogPosToReal(int space, int worldX)
{
	TripleInt pos;

	pos.y = 0;
	pos.z = space / worldX;
	pos.x = space - pos.z * worldX;

	return pos;
}


#ifndef _CONSOLE
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
#else
int main(int argc, char* argv[])
#endif
{

#ifndef _CONSOLE
	glutInit(&nCmdShow, &lpCmdLine);
#else
	glutInit(&argc, argv);
#endif
	


	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize ( INIT_WW, INIT_WH);
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("path."); 
	glShadeModel(GL_SMOOTH);

	//enable lights
	glEnable(GL_LIGHTING);
	//glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );
	glEnable(GL_LIGHT0);	
	glEnable( GL_COLOR_MATERIAL );
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	//glEnable(GL_LIGHT2);
	
	//set up menu
	InitMenu();
  
	Demo* c = new Demo( INIT_WW, INIT_WH );   
  
	transferControlToGLUT();

	return 0;
}


