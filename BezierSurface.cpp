#include "BezierSurface.h"


///////////////////////////////////////////////////////////////////////////////////////////////
// Default Constructor:
// right now this does all the math, but if I ever add functionality, it will have to move
///////////////////////////////////////////////////////////////////////////////////////////////
BezierSurface::BezierSurface(int numUCtrlPts, int numVCtrlPts, 
							 float xSize, float zSize, int xTess, int zTess, int maxCtrlPtY )
{

	m_iNumUCtrlPts = numUCtrlPts;
	m_iNumVCtrlPts = numVCtrlPts;
	m_fXSize = xSize;
	m_fZSize = zSize;
	m_iXTess = xTess;
	m_iZTess = zTess;
	m_iMaxCtrlPtY = maxCtrlPtY;	
	m_bContourColor = true;

	srand( timeGetTime() );

	SetupColors();

	//get scaling factor to choose correct color
	m_fContourScale = (float)NUM_CONTOUR_BANDS / (2 * (float)MAX_TERRAIN_ELEVATION);

	//generate bernstein basis functions
	for(int curU = 0; curU < m_iNumUCtrlPts; curU++)
	{
		m_vBasisFuncU.push_back( *(new BernsteinPoly(m_iNumUCtrlPts-1, curU)) );
	}

	for(int curV = 0; curV < m_iNumVCtrlPts; curV++)
	{
		m_vBasisFuncV.push_back( *(new BernsteinPoly(m_iNumVCtrlPts-1, curV)) );
	}


	TripleFloat *pTriple;
	float stepU = m_fZSize / (float)(m_iNumUCtrlPts - 1);
	float stepV = m_fXSize / (float)(m_iNumVCtrlPts - 1);

	//generate control points
	for(int k = 0; k < m_iNumVCtrlPts; k++)
	{
		for(int l = 0; l < m_iNumUCtrlPts; l++)
		{
			pTriple = new TripleFloat;
			pTriple->z = stepU * l;
			pTriple->x = stepV * k;
			pTriple->y = (float)((rand() % (m_iMaxCtrlPtY * 2)) - m_iMaxCtrlPtY );			
			m_vTerrainCtrlPts.push_back( *pTriple );
		}

	}
	/* hack to plot bernstein poly's
	ofstream dout;
	char fname[20];
	for(int i=0; i < m_iNumUCtrlPts; i++)
	{
		sprintf(fname, "bernpoly%d.txt", i);
		dout.open(fname);
		for(float j=0.0; j<1.0;j+=0.01)
		{
			dout << j << " " << m_vBasisFuncU[i](j) << endl;
		}
		dout.close();
	}
	*/
	CalcVerts();

}

/////////////////////////////////////////////////////////////////////////////////////////////////
//render the surface
/////////////////////////////////////////////////////////////////////////////////////////////////

BezierSurface::Display(bool bShowCtrlPts, bool bShowMesh, GLenum eMode)
{

	int colorIndex;
	int vertIndex1, vertIndex2;

	vector<TripleFloat>::iterator iter;

	//if control points should be shown
	if(bShowCtrlPts)
	{
		//loop through the points and draw a sphere for each
		for(iter = m_vTerrainCtrlPts.begin(); iter != m_vTerrainCtrlPts.end(); ++iter)
		{
			//make em blue
			glColor3f(0,0,1.0);
			glPushMatrix();
			glTranslatef(iter->x,iter->y,iter->z);
			glutSolidSphere(.5,8,8);
			glPopMatrix();
		}

		//if the control points should be connected
		if(bShowMesh)
		{			

			int col, row;

			//draw the line strips in the z direction
			//for each column
			glColor3f(1.0f, 0, 0);		
			for(col = 0; col < m_iNumUCtrlPts; col++)
			{

				glBegin(GL_LINE_STRIP);
				//for each row
				for(row = 0; row < m_iNumVCtrlPts; row++)
				{
						
					//put another point on the line strip
					vertIndex1 = col * m_iNumVCtrlPts + row;														

					glVertex3f( m_vTerrainCtrlPts[vertIndex1].x, 
					m_vTerrainCtrlPts[vertIndex1].y, 
					m_vTerrainCtrlPts[vertIndex1].z);
													
				}
				glEnd();
			}

			//draw the line strips in the x direction
			//for each column
			for(row = 0; row < m_iNumVCtrlPts; row++)
			{

				glBegin(GL_LINE_STRIP);
				//for each row
				for(col = 0; col < m_iNumUCtrlPts; col++)
				{
					//put another point on the line strip													
					vertIndex1 = col * m_iNumVCtrlPts + row;
					
					glColor3f(1.0f, 0, 0);						

					glVertex3f( m_vTerrainCtrlPts[vertIndex1].x, 
					m_vTerrainCtrlPts[vertIndex1].y, 
					m_vTerrainCtrlPts[vertIndex1].z);
													
				}
				glEnd();
			}

		}
	}	
		
	//draw the surface

	//the surface is rendered with individual triangles so that each poly will have a 
	//separate entry in the select buffer, giving finer pick resolution4

	//for each column
	for(int curCol = 0; curCol < (m_iXTess - 1); curCol++)
	{
		
		for(int curRow = 0; curRow < (m_iZTess-1); curRow++)
		{

			
			vertIndex1 = curCol * m_iZTess + curRow;
			vertIndex2 = curCol * m_iZTess + curRow + m_iZTess;

			//begin the first triangle
			if(eMode == GL_SELECT)
				glLoadName( MAX_AGENTS+1+vertIndex1 );
			glBegin( GL_TRIANGLES );					

			//if the contour coloring is on, set a color
			if(m_bContourColor)
			{
				colorIndex = (int)( m_fContourScale * ( m_vTerrainVerts[vertIndex2].y + MAX_TERRAIN_ELEVATION ) );
				glColor3fv( m_ContourColors[colorIndex] );
			}

			//add the bottom two verts and normals for this quad
			glNormal3f( m_vTerrainNorms[vertIndex2].x,
						m_vTerrainNorms[vertIndex2].y,
						m_vTerrainNorms[vertIndex2].z );
			glVertex3f( m_vTerrainVerts[vertIndex2].x, 
						m_vTerrainVerts[vertIndex2].y, 
						m_vTerrainVerts[vertIndex2].z);
			
			//set the color
			if(m_bContourColor)
			{
				colorIndex = (int)( m_fContourScale * ( m_vTerrainVerts[vertIndex1].y + MAX_TERRAIN_ELEVATION ) );
				glColor3fv( m_ContourColors[colorIndex] );
			}
			glNormal3f( m_vTerrainNorms[vertIndex1].x,
						m_vTerrainNorms[vertIndex1].y,
						m_vTerrainNorms[vertIndex1].z );
			glVertex3f( m_vTerrainVerts[vertIndex1].x, 
						m_vTerrainVerts[vertIndex1].y, 
						m_vTerrainVerts[vertIndex1].z);
		

			vertIndex2++;
			//if the contour coloring is on, set a color
			if(m_bContourColor)
			{
				colorIndex = (int)( m_fContourScale * ( m_vTerrainVerts[vertIndex2].y + MAX_TERRAIN_ELEVATION ) );
				glColor3fv( m_ContourColors[colorIndex] );
			}			
			//add the top two verts and normals for this quad
			glNormal3f( m_vTerrainNorms[vertIndex2].x,
						m_vTerrainNorms[vertIndex2].y,
						m_vTerrainNorms[vertIndex2].z );
			glVertex3f( m_vTerrainVerts[vertIndex2].x, 
						m_vTerrainVerts[vertIndex2].y, 
						m_vTerrainVerts[vertIndex2].z);

			glEnd();


			//begin the 2nd triangle
			//if(eMode == GL_SELECT)
			//	glLoadName( MAX_AGENTS+2+vertIndex1*2 );
			glBegin( GL_TRIANGLES );	

			//set a color
			if(m_bContourColor)
			{
				colorIndex = (int)( m_fContourScale * ( m_vTerrainVerts[vertIndex1].y + MAX_TERRAIN_ELEVATION ) );
				glColor3fv( m_ContourColors[colorIndex] );
			}
			glNormal3f( m_vTerrainNorms[vertIndex1].x,
						m_vTerrainNorms[vertIndex1].y,
						m_vTerrainNorms[vertIndex1].z );
			glVertex3f( m_vTerrainVerts[vertIndex1].x, 
						m_vTerrainVerts[vertIndex1].y, 
						m_vTerrainVerts[vertIndex1].z);

			//set a color
			vertIndex1++;
			if(m_bContourColor)
			{
				colorIndex = (int)( m_fContourScale * ( m_vTerrainVerts[vertIndex1].y + MAX_TERRAIN_ELEVATION ) );
				glColor3fv( m_ContourColors[colorIndex] );
			}			
			glNormal3f( m_vTerrainNorms[vertIndex1].x,
						m_vTerrainNorms[vertIndex1].y,
						m_vTerrainNorms[vertIndex1].z );
			glVertex3f( m_vTerrainVerts[vertIndex1].x, 
						m_vTerrainVerts[vertIndex1].y, 
						m_vTerrainVerts[vertIndex1].z);

		
			//if the contour coloring is on, set a color
			if(m_bContourColor)
			{
				colorIndex = (int)( m_fContourScale * ( m_vTerrainVerts[vertIndex2].y + MAX_TERRAIN_ELEVATION ) );
				glColor3fv( m_ContourColors[colorIndex] );
			}

			//add the top two verts and normals for this quad
			glNormal3f( m_vTerrainNorms[vertIndex2].x,
						m_vTerrainNorms[vertIndex2].y,
						m_vTerrainNorms[vertIndex2].z );
			glVertex3f( m_vTerrainVerts[vertIndex2].x, 
						m_vTerrainVerts[vertIndex2].y, 
						m_vTerrainVerts[vertIndex2].z);

			
			//end the triangle
			glEnd();
		}
			
	}
				

}

BezierSurface::CalcVerts()
{
	
	//generate verts
	float stepZ = 1.0 / (float)(m_iZTess - 1);
	float stepX = 1.0 / (float)(m_iXTess - 1);
	float uParam, vParam;
	float bu, bv;
	float dbu, dbv;
	float inverse;
	int ctrlIndex;
	TripleFloat uTan, vTan;
	TripleFloat *pNorm;
	TripleFloat *pTriple;

	//for each vert
	for(int curX = 0; curX < m_iXTess; curX++)
	{
		for(int curZ = 0; curZ < m_iZTess; curZ++)
		{
			vParam = stepX * curX;	
			uParam = stepZ * curZ;
			
			//create a new vert and new tangent vectors
			pTriple = new TripleFloat;			
			pTriple->x = 0.0f;
			pTriple->y = 0.0f;
			pTriple->z = 0.0f;

			uTan.x = 0.0f;
			uTan.y = 0.0f;
			uTan.z = 0.0f;
			
			vTan.x = 0.0f;
			vTan.y = 0.0f;
			vTan.z = 0.0f;

			pNorm = new TripleFloat;

			//sum the influence of each control point on this vert
			for(int curV = 0; curV < m_iNumVCtrlPts; curV++)
			{
				for(int curU = 0; curU < m_iNumUCtrlPts; curU++)
				{
					//get the index into the vector
					ctrlIndex = curV*m_iNumUCtrlPts+curU;

					//get the interpolation factors from the basis functions
					bu = m_vBasisFuncU[curU](uParam);
					bv = m_vBasisFuncV[curV](vParam);

					dbu = m_vBasisFuncU[curU].Derivative(uParam);
					dbv = m_vBasisFuncV[curV].Derivative(vParam);
 					
					//add in this control point's contribution to the vertex
					pTriple->x += m_vTerrainCtrlPts[ctrlIndex].x * bu * bv;
					pTriple->y += m_vTerrainCtrlPts[ctrlIndex].y * bu * bv;
					pTriple->z += m_vTerrainCtrlPts[ctrlIndex].z * bu * bv;	
					
					//add in this control point's contribution to the u tangent
					uTan.x += m_vTerrainCtrlPts[ctrlIndex].x * dbu * bv;
					uTan.y += m_vTerrainCtrlPts[ctrlIndex].y * dbu * bv;
					uTan.z += m_vTerrainCtrlPts[ctrlIndex].z * dbu * bv;	

					//add in this control point's contribution to the v tangent
					vTan.x += m_vTerrainCtrlPts[ctrlIndex].x * bu * dbv;
					vTan.y += m_vTerrainCtrlPts[ctrlIndex].y * bu * dbv;
					vTan.z += m_vTerrainCtrlPts[ctrlIndex].z * bu * dbv;	

				}
			}

			//add the vert to the list
			m_vTerrainVerts.push_back(*pTriple);

			//find the normal and normalize it
			pNorm->x = uTan.y * vTan.z - uTan.z * vTan.y;
			pNorm->y = uTan.z * vTan.x - uTan.x * vTan.z;
			pNorm->z = uTan.x * vTan.y - uTan.y * vTan.x;

			inverse = 1.0f / sqrt( pNorm->x * pNorm->x + pNorm->y * pNorm->y + pNorm->z * pNorm->z );
			pNorm->x *= inverse;
			pNorm->y *= inverse;
			pNorm->z *= inverse;

			m_vTerrainNorms.push_back(*pNorm);						

		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////

TripleFloat BezierSurface::GetVertPos(int index)
{
	return m_vTerrainVerts[index];	
}


////////////////////////////////////////////////////////////////////////////////////////////
void BezierSurface::SetupColors()
{
	m_ContourColors[0][0] = 37;   m_ContourColors[0][1] = 71;  m_ContourColors[0][2] = 133;
	m_ContourColors[1][0] = 51;   m_ContourColors[1][1] = 91;  m_ContourColors[1][2] = 183;
	m_ContourColors[2][0] = 82;   m_ContourColors[2][1] = 125;  m_ContourColors[2][2] = 207;
	m_ContourColors[3][0] = 111;   m_ContourColors[3][1] = 147; m_ContourColors[3][2] = 215;
	m_ContourColors[4][0] = 141;   m_ContourColors[4][1] = 170; m_ContourColors[4][2] = 224;
	m_ContourColors[5][0] = 173;  m_ContourColors[5][1] = 194; m_ContourColors[5][2] = 233;
	m_ContourColors[6][0] = 196;  m_ContourColors[6][1] = 210; m_ContourColors[6][2] = 238;
	m_ContourColors[7][0] = 208; m_ContourColors[7][1] = 220; m_ContourColors[7][2] = 242;
	m_ContourColors[8][0] = 225; m_ContourColors[8][1] = 233; m_ContourColors[8][2] = 247;
	m_ContourColors[9][0] = 253; m_ContourColors[9][1] = 254; m_ContourColors[9][2] = 255;

	for(int i = 0; i < NUM_CONTOUR_BANDS; i++)
	{
		for(int k = 0; k < 3; k++)
		{

			m_ContourColors[i][k] /= 255.0f;
		}


	}
}

