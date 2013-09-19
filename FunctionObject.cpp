/*
#include "FunctionObject.h"

FunctionObject::FunctionObject()
{

}

////////////////////////////////////////
////////////////////////////////////////

//template <class elementType> 
AddToFront::AddToFront()
{

}


////////////////////////////////////////

//template <class elementType> 
void AddToFront::operator()( list<NodeType> &curList, list<NodeType> &newElements )
{
	curList.splice(curList.begin(), newElements, newElements.begin(), newElements.end() );

}



////////////////////////////////////////
////////////////////////////////////////

//template <class elementType> 
AddToBack::AddToBack()
{
	
}

////////////////////////////////////////

//template <class elementType> 
void AddToBack::operator()( list<NodeType> &curList, list<NodeType> &newElements )
{
	curList.splice(curList.end(), newElements, newElements.begin(), newElements.end() );
}

////////////////////////////////////////
////////////////////////////////////////

//template <class elementType> 
GreedySort::GreedySort()
{
	
}

////////////////////////////////////////

//template <class elementType> 
void GreedySort::operator()( list<NodeType> &curList, list<NodeType> &newElements )
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

////////////////////////////////////////
////////////////////////////////////////

AStarSort::AStarSort()
{
	
}

////////////////////////////////////////

//template <class elementType> 
void AStarSort::operator()( list<NodeType> &curList, list<NodeType> &newElements )
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

////////////////////////////////////////
////////////////////////////////////////

AddRandom::AddRandom()
{
	
}

////////////////////////////////////////

//template <class elementType> 
void AddRandom::operator()( list<NodeType> &curList, list<NodeType> &newElements )
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
*/