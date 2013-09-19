#ifndef _AGENT_H
#define _AGENT_H

#include <vector>
#include <list>
#include <algorithm>
#include "BezierSurface.h"
#include "defines.h"
#include "FunctionObject.h"
#include "NodeType.h"
#include <iostream>
#include <windows.h>
using namespace std;

class Agent
{
public:
	Agent(int Pos, int agentNum, int WorldX, int WorldY, int WorldZ );
	~Agent();

	//takes the next move from the planned path
	void Move(vector<unsigned char> &worldMap, BezierSurface &terrain);
	
	//plans a path to the goal
	void PlanPath(vector<unsigned char> &worldMap, BezierSurface &terrain, bool showSearch);

	//get agent's current position
	int GetCurPos();
	//get the agent's last position
	int GetPrevPos();

	//get and set the target position of the agent
	int  GetGoal();
	void SetGoal(int goal);

	//get this agent's index
	int  GetAgentNum();

	//get the list of nodes in the search space
	list<NodeType> &GetSearchSpace();

	//get the path to the goal
	list<int> &GetPath();
	

	//is the agent currently searching
	bool Searching();

	//change the search strategy of the agent
	void SetMoveStrategy(e_MoveStrategy moveStrat);

private:


	void PossibleMoves( int *moves, int curPos ); //pass in an array of length and the current position

	int Distance(int space1, int space2);  //returns manhattan distance between given spaces


	typedef BinaryListProc<NodeType> QFunc;
	
	//given the current world state and a binary_function that adds newly expanded nodes to 
	//the list of previously expanded nodes, finds a path to the goal
	//pass -1 as max depth for infinite search
	void GeneralSearch(vector<unsigned char>&worldMap, BezierSurface &terrain, 
					   QFunc &QueueFunc, bool showSearch, int maxDepth);

	//gets the new search nodes possible after the current node
	list<NodeType> DBExpand(NodeType &curNode, vector<unsigned char> &worldMap, BezierSurface &terrain, int maxDepth);

	list<int> m_lPathToGoal;	   //current path to the goal	
	list<NodeType> m_lSearchSpace; //current search space- member so that the search can be displayed

	int m_iAgentNum; //index of this agent

	int m_iCurPos;   //current agent position
	int m_iPrevPos;  //previous agent position

	int m_iGoal;	 //desired position	

	e_MoveStrategy m_eMoveStrategy; //current movement strategy 

	int m_iWorldX;	//world size in X dim
	int m_iWorldY;  //world size in Y dim
	int m_iWorldZ;  //world size in Z dim


};


#endif