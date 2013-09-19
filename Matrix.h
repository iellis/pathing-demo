
#ifndef _MATRIX_H
#define _MATRIX_H

#include <math.h>

class Matrix
{

public:

	
	Matrix();					//default constructor- identity
	Matrix( Matrix &);			//copy constructor
	Matrix( float *pData );		//data constructor- pointer to 16 float array
	~Matrix();					//destructor

	
	float *GetDataPtr();		//get ptr to data
	void   Reset();				//reset to identity	

	//math ops- these operations create transformation matrices	
	void Scale( float fX, float	fY, float fZ );
	void UniformScale( float fFactor );
	void Translate( float fX, float fY, float fZ );
	void TranslateX( float fX );
	void TranslateY( float fY );
	void TranslateZ( float fZ );
	void RotateX( float fAngle );
	void RotateY( float fAngle );
	void RotateZ( float fAngle );
	void Rotate( float fAngle, float fX, float fY, float fZ );


	//overloaded operators
	Matrix operator*( Matrix );
	void   operator=( Matrix );



private:

	float *m_fData; //matrix data-16 floats long

};


#endif

