#ifndef BERNSTEINPOLY_H
#define BERNSTEINPOLY_H

#include <math.h>

class BernsteinPoly
{

public:

	//the bernstein polynomial is defined as  (n! / i!(n-i)!) * u^i * (1 - u )^(n-i) 


	//constructor, must provide the order of the function
	BernsteinPoly(int ordN, int ordI);

	//evaluate the function for the given parameter
	float operator()(float u);


	float Derivative(float u);

	private:

	int Factorial(int num, int result);
	int m_iOrderN;
	int m_iOrderI;
	int m_iNChooseI;

};

#endif