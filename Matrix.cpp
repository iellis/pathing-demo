#include "Matrix.h"

//////////////////////////////////////////////////////////

Matrix::Matrix()
{	
	m_fData = new float[16];

	//init to 0
	for(int i=0; i < 16;i++)
		m_fData[i] = 0.0;
	
	//set to identity
	m_fData[0] = 1.0;
	m_fData[5] = 1.0;	
	m_fData[10] = 1.0;
	m_fData[15] = 1.0;
		
}

//////////////////////////////////////////////////////////

Matrix::Matrix( Matrix &Src )
{
	//get src data
	float *pData = Src.GetDataPtr();
	
	//copy it	
	for(int i=0; i < 16;i++)
		m_fData[i] = pData[i];
}

//////////////////////////////////////////////////////////

Matrix::Matrix( float *pData )
{
	//copy the ptr
	m_fData = pData;
	
}

//////////////////////////////////////////////////////////

Matrix::~Matrix()
{
	delete m_fData;	
}

//////////////////////////////////////////////////////////

float *Matrix::GetDataPtr()
{
	return m_fData;
}

//////////////////////////////////////////////////////////

void Matrix::Reset()
{
	//init to 0
	for(int i=0; i < 16;i++)
		m_fData[i] = 0.0;
	
	//set to identity
	m_fData[0] = 1.0;
	m_fData[5] = 1.0;	
	m_fData[10] = 1.0;
	m_fData[15] = 1.0;
}

//////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////

void Matrix::Scale( float fX, float	fY, float fZ )
{
	Reset();

	m_fData[0] = fX;
	m_fData[5] = fY;
	m_fData[10] = fZ;
}

//////////////////////////////////////////////////////////

void Matrix::UniformScale( float fFactor )
{
	Reset();

	m_fData[0] = fFactor;
	m_fData[5] = fFactor;
	m_fData[10] = fFactor;
	
}

//////////////////////////////////////////////////////////

void Matrix::Translate( float fX, float fY, float fZ )
{
	Reset();

	m_fData[3] = fX;
	m_fData[7] = fY;
	m_fData[11] = fZ;
}

//////////////////////////////////////////////////////////

void Matrix::TranslateX( float fX )
{
	Reset();
	m_fData[3] = fX;
}

//////////////////////////////////////////////////////////

void Matrix::TranslateY( float fY )
{
	Reset();
	m_fData[7] = fY;
}

//////////////////////////////////////////////////////////

void Matrix::TranslateZ( float fZ )
{
	Reset();
	m_fData[11] = fZ;
}

//////////////////////////////////////////////////////////

void Matrix::RotateX( float fAngle )
{
	Reset();
	m_fData[5] = (float)cos(fAngle);
	m_fData[6] = (float)-sin(fAngle);
	m_fData[9] = (float)sin(fAngle);
	m_fData[10] = (float)cos(fAngle);
}

//////////////////////////////////////////////////////////

void Matrix::RotateY( float fAngle )
{
	Reset();
	m_fData[0] = (float)cos(fAngle);
	m_fData[2] = (float)sin(fAngle);
	m_fData[8] = (float)-sin(fAngle);
	m_fData[10] = (float)cos(fAngle);
}

//////////////////////////////////////////////////////////

void Matrix::RotateZ( float fAngle )
{
	Reset();
	m_fData[0] = (float)cos(fAngle);
	m_fData[1] = (float)-sin(fAngle);
	m_fData[4] = (float)sin(fAngle);
	m_fData[5] = (float)cos(fAngle);
}

//////////////////////////////////////////////////////////

void Matrix::Rotate( float fAngle, float fX, float fY, float fZ )
{
	Reset();

}

//////////////////////////////////////////////////////////

Matrix Matrix::operator*( Matrix Op )
{

	Matrix NewMat;
	float *NewMatData = NewMat.GetDataPtr();

	//get data for 2nd matrix
	float *OpData = Op.GetDataPtr();

	//for each row of mat1
	for(int row = 0; row < 4; row++)
	{
		//for each column of mat2
		for(int col = 0; col < 4; col++)
		{
			//sum the products down the row * column
			for(int mul = 0; mul < 4; mul++)
			{
				NewMatData[row*4+col] += m_fData[row*4+mul] * OpData[col+mul*4];			
			}
		}
	}
	
	return NewMat;
}

//////////////////////////////////////////////////////////

void Matrix::operator=( Matrix Op)
{
	float *OpData = Op.GetDataPtr();

	for(int i = 0; i < 16; i++)
		m_fData[i] = OpData[i];
}