#ifndef BEZIERSURFACE_H
#define BEZIERSURFACE_H

#include <GL/glut.h>
#include <vector>
#include <windows.h>
#include <fstream>
#include "BernsteinPoly.h"
#include "defines.h"

using namespace std;

class BezierSurface
{

public:
	
	//Constructor
	//U must be greater than or equal to V!!!
	BezierSurface(int numUCtrlPts, int numVCtrlPts, 
				  float xSize, float zSize, int xTess, int zTess, int maxCtrlPtY);	
	
	//render the surface 
	//if GL_SELECT is passed as mode, the name of each square is loaded as it is rendered
	Display(bool bShowCtrlPts, bool bShowMesh, GLenum eMode);
	//calculate the vertices of the surface
	CalcVerts();	
	//returns the coordinates of a logical (vertex) position
	TripleFloat GetVertPos( int index );

private:

	void SetupColors();						//initialize the contour band colors

	vector <TripleFloat> m_vTerrainVerts;   //terrain vertices
	vector <TripleFloat> m_vTerrainNorms;   //terrain normals
	vector <TripleFloat> m_vTerrainCtrlPts; //control points for terrain
	vector <BernsteinPoly> m_vBasisFuncU;	//basis functions for u direction
	vector <BernsteinPoly> m_vBasisFuncV;   //basis functions for v direction

	bool  m_bContourColor;					//set the color based on the y value
	float m_ContourColors[NUM_CONTOUR_BANDS][3];	//rgb colors for the contour bands

	float m_fContourScale;
	int   m_iNumUCtrlPts;
	int   m_iNumVCtrlPts;
	int   m_iMaxCtrlPtY;
	float m_fXSize;
	float m_fZSize;
	int   m_iXTess;
	int   m_iZTess;

};



#endif