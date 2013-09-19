#include "BezierSurface.h"


///////////////////////////////////////////////////////////////////////////////////////////////
// Default Constructor:
// right now this does all the math, but if I ever add functionality, it will have to move
///////////////////////////////////////////////////////////////////////////////////////////////
BezierSurface::BezierSurface(int numUCtrlPts, int numVCtrlPts, 
							 float xSize, float zSize, int xTess, int zTess, int maxCtrlPtY );
{

	m_iNumUCtrlPts = numUCtrlPts;
	m_inumVCtrlPts = numVCtrlPts;
	m_fXSize = xSize;
	m_fZSize = zSize;
	m_iXTess = xTess;
	m_iZTess = zTess;
	m_iMaxCtrlPtY = maxCtrlPtY;



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
	float stepU = (float)m_iZSize / (float)(m_iNumUCtrlPts - 1);
	float stepV = (float)m_iXSize / (float)(m_iNumVCtrlPts - 1);

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
			pNorm->x *= -inverse;
			pNorm->y *= -inverse;
			pNorm->z *= -inverse;

			m_vTerrainNorms.push_back(*pNorm);						

		}
	}


}

/////////////////////////////////////////////////////////////////////////////////////////////////
//render the surface
/////////////////////////////////////////////////////////////////////////////////////////////////

BezierSurface::Display()
{
	vector<TripleFloat>::iterator iter;
	for(iter = m_vTerrainCtrlPts.begin(); iter != m_vTerrainCtrlPts.end(); ++iter)
	{
		glPushMatrix();
		//iter->y = 0;
		glTranslatef(iter->x,iter->y,iter->z);
		glutSolidSphere(.5,5,5);
		glPopMatrix();
	}

 
	int vertIndex1, vertIndex2;
	
	glPushMatrix();
	
	for(int curCol = 0; curCol < (m_iXTess - 1); curCol++)
	{
		glBegin( GL_TRIANGLE_STRIP );
		for(int curRow = 0; curRow < m_iZTess; curRow++)
		{
			vertIndex1 = curCol * m_iZTess + curRow;
			vertIndex2 = curCol * m_iZTess + curRow + m_iZTess;

			
			glNormal3f( m_vTerrainNorms[vertIndex1].x,
						m_vTerrainNorms[vertIndex1].y,
						m_vTerrainNorms[vertIndex1].z );
			glVertex3f( m_vTerrainVerts[vertIndex1].x, 
						m_vTerrainVerts[vertIndex1].y, 
						m_vTerrainVerts[vertIndex1].z);
			glNormal3f( m_vTerrainNorms[vertIndex2].x,
						m_vTerrainNorms[vertIndex2].y,
						m_vTerrainNorms[vertIndex2].z );
			glVertex3f( m_vTerrainVerts[vertIndex2].x, 
						m_vTerrainVerts[vertIndex2].y, 
						m_vTerrainVerts[vertIndex2].z);
			
		}
		glEnd();
	}
			
	glPopMatrix();

}

BezierSurface::Recalculate()
{

}