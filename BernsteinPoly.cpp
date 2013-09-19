#include "BernsteinPoly.h"

BernsteinPoly::BernsteinPoly(int ordN, int ordI)
{
	m_iOrderN = ordN;
	m_iOrderI = ordI;
	
	int nFact;
	int iFact;
	int nsubiFact;
	
	nFact = Factorial(m_iOrderN, m_iOrderN);
	iFact = Factorial(m_iOrderI, m_iOrderI);
	nsubiFact = Factorial(m_iOrderN - m_iOrderI, m_iOrderN - m_iOrderI);
	m_iNChooseI = nFact / (iFact * nsubiFact);
}

float BernsteinPoly::operator()(float u)
{
	

	return (float)( m_iNChooseI * pow(u, m_iOrderI) * pow( 1 - u, m_iOrderN - m_iOrderI ) );
	
}

float BernsteinPoly::Derivative(float u)
{

	
	if(m_iOrderI == 0)
		return( m_iOrderN * (float)pow( 1.0f - u, m_iOrderN - 1) );

	else if( m_iOrderI == m_iOrderN )
		return( m_iOrderI * u );

	else
		return( m_iNChooseI * ( m_iOrderI * (float)pow(u, m_iOrderI - 1) * 
								(float)pow( 1.0f - u, m_iOrderN - m_iOrderI ) -
								(float)pow( u, m_iOrderI ) * (m_iOrderN - m_iOrderI) * 
								(float)pow( 1.0f - u, (m_iOrderN - m_iOrderI) - 1)));
}


int BernsteinPoly::Factorial(int num, int result)
{

	if(num == 0)
		return 1;
	else if( num == 1)
		return result;

	else
	{
		num--;
		return Factorial( num, result*num );
	}

}
