/* Copyright Steve Rabin, 2008.
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright Steve Rabin, 2008"
 */

#pragma once


class GameObject;

typedef std::list<D3DXVECTOR3> WaypointList;

enum MovementMode
{
	MOVEMENT_NULL,
	MOVEMENT_SEEK_TARGET,
	MOVEMENT_WAYPOINT_LIST
};


class A_Star_Node
{
public:

	//Default constructor
	A_Star_Node() :m_cost(0.0f), m_givenCost(0.0f), m_nodePos(), m_parentNode() {}

	//Non default constructor
	A_Star_Node(float cost_, float m_givenCost, D3DXVECTOR2 nodePos_, D3DXVECTOR2 parentNode_) 
	:m_cost(cost_), m_givenCost(m_givenCost),m_nodePos(nodePos_), m_parentNode(parentNode_) {}

	float m_cost; //Total cost, f(x)= g(x) + (h(x)*weight)
	float m_givenCost; //g(x)
	D3DXVECTOR2 m_nodePos; //Node position r,c
	D3DXVECTOR2 m_parentNode; //Parent position r,c
};


struct D3DXVECTOR2_hash
{
	std::size_t operator () (const D3DXVECTOR2& p) const
	{
		auto h1 = p.x;
		auto h2 = p.y;

		return (std::size_t)(h1 * g_terrain.GetWidth() + h2);
	}
};

class A_Star
{
public:

	//Compute the heuristic cost h(x)
	float ComputeHeuristicCost(D3DXVECTOR2 & currentNode_, D3DXVECTOR2 goalNode_, int m_heuristicCalc_);

	//Compute the given cost g(x)
	float ComputeGivenCost(A_Star_Node & parentNode_, D3DXVECTOR2 & currentNode_);

	std::unordered_map<D3DXVECTOR2 , A_Star_Node, D3DXVECTOR2_hash> m_openList; //Vector for the openlist points
	std::unordered_map<D3DXVECTOR2 , A_Star_Node, D3DXVECTOR2_hash> m_closedList;//Vector for the closed list points
	std::vector<D3DXVECTOR2> m_wayPointListVector;//Vector for the waypoint list points
};

class Movement
{
	friend class PathfindingTests;

public:
	Movement(GameObject& owner);
	~Movement(void);

	inline void SetTarget(D3DXVECTOR3& target)			{ m_target = target; }
	inline D3DXVECTOR3& GetTarget(void)					{ return(m_target); }

	void Animate(double dTimeDelta);
	void DrawDebugVisualization(IDirect3DDevice9* pd3dDevice, D3DXMATRIX* pViewProj);

	void SetIdleSpeed(void);
	void SetWalkSpeed(void);
	void SetJogSpeed(void);

	void UpdateMovementSetting(void);
	void ResetMovementSetting(void);

	bool ComputePathWithTiming(int r, int c, bool newRequest);

	// clear waypointlist
	void ClearWaypointList(void);

#if defined (EXTRACREDIT_FOGOFWAR)
	bool GetFogOfWar() const                                { if (m_extracredit == EXTRA_FogOfWar) { return true; } return false; }
#endif

	void SetHeuristicWeight(float value)					{ m_heuristicWeight = value; }
	float GetHeuristicWeight() const                        { return m_heuristicWeight; }
	void SetHeuristicCalc(int value)						{ m_heuristicCalc = value; }
	int GetHeuristicCalc() const                            { return m_heuristicCalc; }
	void SetSmoothPath(bool enable)						{ m_smooth = enable; }
	bool GetSmoothPath() const                              { return m_smooth; }
	void SetRubberbandPath(bool enable)					{ m_rubberband = enable; }
	bool GetRubberbandPath() const                          { return m_rubberband; }
	void SetStraightlinePath(bool enable)					{ m_straightline = enable; }
	bool GetStraightlinePath() const                        { return m_straightline; }
	void SetSingleStep(bool enable)					    { m_singleStep = enable; }
	bool GetSingleStep() const                              { return m_singleStep; }
	void SetExtraCredit(int value)						    { m_extracredit = value; }
	int GetExtraCredit() const							    { return m_extracredit; }
	void AStarUsesAnalysis(bool enable)					{ m_aStarUsesAnalysis = enable; }
	bool GetAnalysis() const                                { return m_aStarUsesAnalysis; }
	void SetDebugDraw(bool enable)							{ m_debugDraw = enable; }
	bool GetDebugDraw() const								{ return m_debugDraw; }

protected:

	GameObject* m_owner;

	bool m_smooth;
	bool m_rubberband;
	bool m_straightline;
	bool m_singleStep;
	int m_extracredit;
	bool m_aStarUsesAnalysis;
	float m_heuristicWeight;
	int m_heuristicCalc;
	MovementMode m_movementMode;

	D3DXVECTOR3 m_target;
	D3DXVECTOR3 m_goal;

	WaypointList m_waypointList;

	float m_speedWalk;
	float m_speedJog;

	bool m_debugDraw;

	bool ComputePath(int r, int c, bool newRequest);
};
