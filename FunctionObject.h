#ifndef _FUNCTIONOBJECT_H
#define _FUNCTIONOBJECT_H

#include <list>
#include "defines.h"
#include "NodeType.h"
#include <functional>
using namespace std;


/*
template < class _Tx >
struct AddFront : public binary_function< _Tx, _Tx, int >
{
	operator()( list<_Tx> &curList, list<_Tx> &newElements )
	{
		curList.splice(curList.begin(), newElements, newElements.begin(), newElements.end() );
	}
};

////////////////////////////////////////

template <class _Tx> 
struct AddBack : public binary_function< _Tx, _Tx, int >
{
	operator()( list<_Tx> &curList, list<_Tx> &newElements )
	{
		curList.splice(curList.end(), newElements, newElements.begin(), newElements.end() );
	}
};

////////////////////////////////////////

template <class _Tx> 
struct AddGreedy : public binary_function< _Tx, _Tx, int >
{
	operator()( list<_Tx> &curList, list<_Tx> &newElements )
	{
		list<NodeType>::iterator newElem, curElem;	
		//for each new node
		for(newElem = newElements.begin(); newElem !=newElements.end(); ++newElem)
		{
			//get an iterator pointing to the correct insertion spot
			for(curElem = curList.begin(); curElem->DistanceLessThan(*newElem) && curElem != curList.end(); ++curElem);

			//add the node
			curList.insert( curElem, *newElem );							
		}
	}
};

////////////////////////////////////////

template <class _Tx> 
struct AddAStar : binary_function< _Tx, _Tx, int >
{
	operator()( list<_Tx> &curList, list<_Tx> &newElements )
	{
		list<NodeType>::iterator newElem, curElem;	
		//for each new node
		for(newElem = newElements.begin(); newElem !=newElements.end(); ++newElem)
		{
			//get an iterator pointing to the correct insertion spot
			for(curElem = curList.begin(); curElem->CostLessThan(*newElem) && curElem != curList.end(); ++curElem);

			//add the node
			curList.insert( curElem, *newElem );							
		}
	}
};
	
template <class _Tx> 
struct AddRandom : binary_function< _Tx, _Tx, int >
{
	operator()( list<_Tx> &curList, list<_Tx> &newElements )
	{
		int pos = 0;

		list<NodeType>::iterator cur;
		//for each new element
		for(cur = newElements.begin(); cur != newElements.end(); ++cur)
		{
			//get a random position
			if(curList.size() != 0)
				pos = rand() % curList.size();

			//find the position
			list<NodeType>::iterator curListPos = curList.begin();
			for(int i = 1; i < pos; i++)
			{
				++curListPos;
			}

			//store the node there
			curList.insert(curListPos, *cur);
		}
	}
};


#endif

*/






//generic base class for binary list procedures
template <class _Tx>
class BinaryListProc
{

public:

	virtual void operator()(list<_Tx> &, list<_Tx> & )=0;
private:

};

////////////////////////////////////////////////////////////////////////

template <class _Tx> class AddFront : public BinaryListProc<_Tx>
{
public:

	AddFront(){};
	void operator()( list<_Tx> &curList, list<_Tx> &newElements )
    { 
		curList.splice(curList.begin(), newElements, newElements.begin(), newElements.end() );
	}

};

////////////////////////////////////////////////////////////////////////

template <class _Tx> class AddBack : public BinaryListProc<_Tx>
{
public:

	void operator()( list<_Tx> &curList, list<_Tx> &newElements )
	{
		curList.splice(curList.end(), newElements, newElements.begin(), newElements.end() );	
	}

};

////////////////////////////////////////////////////////////////////////

template <class _Tx> class AddGreedy : public BinaryListProc<_Tx>
{
public:

	
	void operator()( list<_Tx> &curList, list<_Tx> &newElements )
	{
		list<NodeType>::iterator newElem, curElem;	
		//for each new node
		for(newElem = newElements.begin(); newElem !=newElements.end(); ++newElem)
		{
			//get an iterator pointing to the correct insertion spot
			for(curElem = curList.begin(); curElem->DistanceLessThan(*newElem) && curElem != curList.end(); ++curElem);

			//add the node
			curList.insert( curElem, *newElem );							
		}
	}
};

////////////////////////////////////////////////////////////////////////
template <class _Tx> class AddAStar : public BinaryListProc<_Tx>
{
public:
	
	void operator()( list<_Tx> &curList, list<_Tx> &newElements )
	{
		list<NodeType>::iterator newElem, curElem;	
		//for each new node
		for(newElem = newElements.begin(); newElem !=newElements.end(); ++newElem)
		{
			//get an iterator pointing to the correct insertion spot
			for(curElem = curList.begin(); curElem->CostLessThan(*newElem) && curElem != curList.end(); ++curElem);

			//add the node
			curList.insert( curElem, *newElem );							
		}
	}

};

////////////////////////////////////////////////////////////////////////

template <class _Tx> class AddRandom : public BinaryListProc<_Tx>
{
public:
	
	void operator()( list<_Tx> &curList, list<_Tx> &newElements )
	{
		int pos = 0;

		list<NodeType>::iterator cur;
		//for each new element
		for(cur = newElements.begin(); cur != newElements.end(); ++cur)
		{
			//get a random position
			if(curList.size() != 0)
				pos = rand() % curList.size();

			//find the position
			list<NodeType>::iterator curListPos = curList.begin();
			for(int i = 1; i < pos; i++)
			{
				++curListPos;
			}

			//store the node there
			curList.insert(curListPos, *cur);
		}
	}	
};
#endif


    