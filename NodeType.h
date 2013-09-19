#ifndef _NODETYPE_H
#define _NODETYPE_H

#include <list>

class NodeType
{
public:
	//constructor
	NodeType();	
	NodeType(int position, int distance, float cost, std::list<int> &ancestors);
	//copy constructor
	NodeType(const NodeType &);

	int DistanceLessThan(const NodeType &) const;
	int CostLessThan(const NodeType &) const;

	//operator<(const NodeType &) const;
	operator==(const NodeType &) const;
	operator==(const int) const;

	//members
	int Position;				//location of space represented by the node	
	int distToGoal;				//minimum distance to goal in # of moves
	float nodeCost;				//cost of reaching this node	
	std::list<int> Ancestors;	//route taken to get to this node

};

#endif