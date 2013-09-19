#include "NodeType.h"

NodeType::NodeType()
{
}

//////////////////////////////////////////////////////////////

NodeType::NodeType(int position, int dist, float cost, std::list<int> &newAncestors) : 
		  Position( position ), Ancestors( newAncestors ), distToGoal( dist ), nodeCost( cost )
{
	
}

//////////////////////////////////////////////////////////////

NodeType::NodeType( const NodeType &oldNode )
{
	Ancestors = oldNode.Ancestors;
	distToGoal = oldNode.distToGoal;
	nodeCost = oldNode.nodeCost;
	Position = oldNode.Position;

}

//////////////////////////////////////////////////////////////
	
NodeType::DistanceLessThan(const NodeType &node) const
{
	return ( this->nodeCost < node.nodeCost );		
}

//////////////////////////////////////////////////////////////

NodeType::CostLessThan(const NodeType &node) const
{
	return ( (this->nodeCost + this->distToGoal) < (node.nodeCost + node.distToGoal) );		
}

//////////////////////////////////////////////////////////////

NodeType::operator==(const NodeType &node) const
{
	return ( this->Position == node.Position );		
}

//////////////////////////////////////////////////////////////
NodeType::operator==(const int pos) const
{
	return (pos == this->Position);
}