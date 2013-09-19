#include "Agent.h"


extern float g_fClimbingPenalty;

Agent::Agent(int Pos, int agentNum, int WorldX, int WorldY, int WorldZ )
{
	m_iCurPos = Pos;
	m_iPrevPos = Pos;
	m_iGoal = Pos;

	m_iAgentNum = agentNum;	

	m_eMoveStrategy = A_STAR;

	m_iWorldX = WorldX;
	m_iWorldY = WorldY;
	m_iWorldZ = WorldZ;
}

////////////////////////////////////////////////////////////

Agent::~Agent()
{
	
}

////////////////////////////////////////////////////////////
void Agent::Move(vector<unsigned char> &worldMap, BezierSurface &terrain)
{
	int newPos;

	//if there is a path, take the next step
	if( !m_lPathToGoal.empty() )
	{
		//get the next step
		newPos = m_lPathToGoal.front();

		if( worldMap.at(newPos) == CLEAR_SPACE )
		{
			m_lPathToGoal.pop_front();

			//swap new and old pos
			m_iPrevPos = m_iCurPos;
			m_iCurPos = newPos;

			//fix the map
			worldMap[m_iPrevPos] = CLEAR_SPACE;
			worldMap[m_iCurPos] = (unsigned char)m_iAgentNum;
		}		

	}
}

////////////////////////////////////////////////////////////
void Agent::PlanPath(vector<unsigned char> &worldMap, BezierSurface &terrain, bool showSearch)
{
	m_lPathToGoal.clear();
	

	AddFront<NodeType> frontAdd;
	AddBack<NodeType> backAdd;
	AddRandom<NodeType> randomAdd;
	AddAStar<NodeType> aStarAdd;
	AddGreedy<NodeType> greedyAdd;
	
	static int curDepth = 1;


	switch(m_eMoveStrategy)
	{
	case RANDOM:		
		
		GeneralSearch(worldMap, terrain, randomAdd, showSearch, -1);
		break;
	
	case DEPTH_FIRST:
		
		GeneralSearch(worldMap, terrain, frontAdd, showSearch, -1);
		break;
	
	case BREADTH_FIRST:
		
		GeneralSearch(worldMap, terrain, backAdd, showSearch, -1);
		break;
	
	case ITERATIVE_DEEPENING:
		
		if( !Searching() )
			curDepth = 1;
			
			GeneralSearch(worldMap, terrain, frontAdd, showSearch, curDepth);
			curDepth++;		
		break;
	
	case GREEDY:
		
		GeneralSearch(worldMap, terrain, greedyAdd, showSearch, -1);
	break;

	case A_STAR:
		
		GeneralSearch(worldMap, terrain, aStarAdd, showSearch, -1);
	break;

	default:
		break;

	}

	
}



////////////////////////////////////////////////////////////

int Agent::GetCurPos()
{
	return m_iCurPos;
}

////////////////////////////////////////////////////////////

int Agent::GetPrevPos()
{
	return m_iPrevPos;
}

////////////////////////////////////////////////////////////

int Agent::GetGoal()
{
	return m_iGoal;
}

////////////////////////////////////////////////////////////

void Agent::SetGoal(int goal)
{
	if(goal >= 0 && goal < m_iWorldX * m_iWorldY *m_iWorldZ)
	{
		m_iGoal = goal;
		m_lSearchSpace.clear();
	}

}

////////////////////////////////////////////////////////////

int Agent::GetAgentNum()
{
	return m_iAgentNum;
}

////////////////////////////////////////////////////////////

list<NodeType> &Agent::GetSearchSpace()
{
	return m_lSearchSpace;
}

////////////////////////////////////////////////////////////

list<int> &Agent::GetPath()
{
	return m_lPathToGoal;
}

////////////////////////////////////////////////////////////
bool Agent::Searching()
{
	return !m_lSearchSpace.empty();
}

////////////////////////////////////////////////////////////
void Agent::SetMoveStrategy(e_MoveStrategy moveStrat)
{
	m_eMoveStrategy = moveStrat;
}



void Agent::PossibleMoves( int *moves, int pos )
{
	//set all moves to current pos
	for(int i=0; i < 8; i++)
		moves[i] = pos;

	//up
	if( ((pos + 1) % m_iWorldZ) )		
		moves[0] = pos+1;

	//down
	if( ((pos) % m_iWorldZ) )		
		moves[1] = pos-1;
	
	//left
	if( !((pos - m_iWorldZ) < 0) )		
		moves[2] = pos - m_iWorldZ;

	//right	
	int wsize = m_iWorldX * m_iWorldZ;
	
	//if going right isn't off the map, return the right value
	if( !((pos + m_iWorldZ) >= wsize) )		
		moves[3] = pos + m_iWorldZ;

	//up-left
	if( (moves[2] != pos) && (moves[0] != pos) )
		moves[4] = moves[2] + 1;

	//down-left
	if( (moves[2] != pos) && (moves[1] != pos) )
		moves[5] = moves[2] - 1;

	//up-right
	if( (moves[3] != pos) && (moves[0] != pos) )
		moves[6] = moves[3] + 1;

	//down-right
	if( (moves[3] != pos) && (moves[1] != pos) )
		moves[7] = moves[3] + 1;

}
////////////////////////////////////////////////////////////

int Agent::Distance(int space1, int space2)
{
	TripleInt pos1 = LogPosToReal(space1, m_iWorldX);
	TripleInt pos2 = LogPosToReal(space2, m_iWorldX);

	int distX = abs(pos1.x - pos2.x);
	int distY = abs(pos1.z - pos2.z);	
	

	return distX > distY ? distX : distY;
	
}



////////////////////////////////////////////////////////////

void Agent::GeneralSearch(vector<unsigned char>&worldMap, BezierSurface &terrain, 
						  QFunc &QueueFunc, bool showSearch, int maxDepth)
{
	//create the search space list
		
	NodeType curNode;

	//set the starting point of the search
	
	if( !Searching() )
	{
		int dist = Distance(this->m_iCurPos, this->m_iGoal);
		list <int>emptyAncestors;
		NodeType start(m_iCurPos, dist, dist, emptyAncestors);
		m_lSearchSpace.push_front(start);	
	}


	//get the first node
	curNode = m_lSearchSpace.front();
	m_lSearchSpace.pop_front();

	//if this node is the goal
	if( m_iGoal == curNode.Position )
	{
		//set the path
		curNode.Ancestors.push_back( curNode.Position );
		m_lPathToGoal.swap(curNode.Ancestors);
		m_lSearchSpace.clear();	
			
		//print the move strategy used
		switch(m_eMoveStrategy)
		{
		case RANDOM:
			cout << "Using Random Search," << endl;
			break;
		
		case DEPTH_FIRST:
			cout << "Using Depth First Search, " << endl;
			break;
		
		case BREADTH_FIRST:
			cout << "Using Breadth First Search, " << endl;
			break;
		
		case ITERATIVE_DEEPENING:
			cout << "Using Iterative Deepending Search, " << endl;		
			break;
		
		case GREEDY:
			cout << "Using Greedy Search, " << endl;
		break;

		case A_STAR:
			cout << "Using A* with optimality parameter of " << g_fClimbingPenalty << endl;
		break;

		default:
			break;

		}

		//find vertical distance traveled
		list<int>::iterator pos;		
		float curY, prevY;
		float vertDist = 0.0f;
		pos = m_lPathToGoal.begin();
		prevY = terrain.GetVertPos( *pos ).y;
		pos++;
		while(pos != m_lPathToGoal.end())
		{						
			curY = terrain.GetVertPos( *pos ).y;
			vertDist += fabs( curY - prevY );
			prevY = curY;
			++pos;
		}
		

		cout << "Vertical Distance Traveled by Agent " << m_iAgentNum << ": " << vertDist << endl;
		cout << "Number of Steps Taken by Agent " << m_iAgentNum << ": " << m_lPathToGoal.size() << endl << endl;

		//remove first node since agent is already there
		m_lPathToGoal.pop_front();
		
		
	}
	else
	{
		//expand the current node and add the children to the search space according to the 
		//queueing function
			
		QueueFunc( m_lSearchSpace, DBExpand( curNode, worldMap, terrain, maxDepth ) );
	}
	
}

////////////////////////////////////////////////////////////

list<NodeType> Agent::DBExpand( NodeType &curNode, vector<unsigned char>&worldMap, BezierSurface &terrain, int maxDepth )
{
	int nodePos[8];
	list<NodeType> newNodes;
	int prevNode, goalDist;
	float nodeCost;//, estimatedCost;
	NodeType *node;

	//if not exceeding the depth limitation
	if( curNode.Ancestors.size() != maxDepth )
	{
		//get the previous node
		if(!curNode.Ancestors.empty())
			prevNode = curNode.Ancestors.back();

		//for all children, this node will be an ancestor, so add it to the list
		curNode.Ancestors.push_back( curNode.Position );

		//get possible moves
		PossibleMoves( nodePos, curNode.Position );

		//for each of the possible new nodes
		for(int i = 0; i < 8; i++)
		{
			if(worldMap.at(nodePos[i]) != BLOCKED_SPACE && //if not blocked
			   nodePos[i] != curNode.Position &&		   //not the same as the current position(off the map)
			   curNode.Ancestors.end() ==				   //and not retracing or crossing your path
			   find( curNode.Ancestors.begin(), curNode.Ancestors.end(), nodePos[i] ) &&
			   m_lSearchSpace.end() == find( m_lSearchSpace.begin(), m_lSearchSpace.end(), nodePos[i] ))			   
			{
				
				
				//distance from this node to the goal
				goalDist = Distance( nodePos[i], m_iGoal );

				//cost of a node = 1.0 + the difference in height from the previous node + cost of reaching the previous node
				//this cost is for A*
				nodeCost = (float)fabs( (terrain.GetVertPos( curNode.Position ).y - 
										 terrain.GetVertPos( nodePos[i] ).y ) )* g_fClimbingPenalty
										+ curNode.nodeCost + 1.0;//- (float)curNode.distToGoal + 1.0;

				//A* uses the total current cost + the minimum additional cost necessary
				//estimatedCost = nodeCost;// + (float)goalDist;							

				//create a new node with distance to goal and the A* cost as its metrics
				node = new NodeType( nodePos[i], goalDist, nodeCost/*estimatedCost*/, curNode.Ancestors );

				newNodes.push_front( *node );

			}
		}

	}
	return newNodes;
}

