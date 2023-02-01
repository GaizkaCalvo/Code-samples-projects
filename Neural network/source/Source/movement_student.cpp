/* Copyright Steve Rabin, 2012. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright Steve Rabin, 2012"
 */

#include <Stdafx.h>

//== operator for the std::find
bool operator==(const A_Star_Node& lhs, const D3DXVECTOR2& vector_) 
{
	return lhs.m_nodePos.x == vector_.x && lhs.m_nodePos.y == vector_.y;
}

float A_Star::ComputeHeuristicCost(D3DXVECTOR2& currentNode_, D3DXVECTOR2 goalNode_, int m_heuristicCalc_)
{
	//Compute the x and y differences from the current node to the goal node
	float xDiff = std::fabs(goalNode_.x - currentNode_.x);
	float yDiff = std::fabs(goalNode_.y - currentNode_.y);

	//Variable to store the cost
	float cost = 0.0f;

	//Euclidean
	if (m_heuristicCalc_ == 0)
	{
		cost = sqrtf((xDiff * xDiff) + (yDiff * yDiff));
	}
	//Octile
	if (m_heuristicCalc_ == 1)
	{
		cost = min(xDiff, yDiff) * sqrtf(2.0f) + max(xDiff, yDiff) - min(xDiff, yDiff);
	}

	//Chebyshev
	if (m_heuristicCalc_ == 2)
	{
		cost = max(xDiff, yDiff);
	}

	//Manhattan
	if (m_heuristicCalc_ == 3)
	{
		cost = xDiff + yDiff;
	}

	//Return cost value
	return cost;
}

float A_Star::ComputeGivenCost(A_Star_Node& parentNode_, D3DXVECTOR2& currentNode_)
{
	//Compute the x and y differences from the current node to the parent node
	float xDiff = std::fabs(currentNode_.x - parentNode_.m_nodePos.x);
	float yDiff = std::fabs(currentNode_.y - parentNode_.m_nodePos.y);

	//Variable to store the cost
	float cost = 0.0f;

	//Diagonal position
	if (xDiff == 0 || yDiff == 0)
		cost = 1.0f;

	//Other positions nodes
	else
		cost = sqrtf(2.0f);


	//Return the computed cost added to the parent cost
	return parentNode_ .m_givenCost + cost;
}

bool Movement::ComputePath( int r, int c, bool newRequest )
{
	//Get the xz coordinates of the goal
	m_goal = g_terrain.GetCoordinates( r, c );
	m_movementMode = MOVEMENT_WAYPOINT_LIST;

	// project 2: change this flag to true
	bool useAStar = true;

	//Check if use A* is activated
	if (useAStar)
	{
		///////////////////////////////////////////////////////////////////////////////////////////////////
		//INSERT YOUR A* CODE HERE
		//1. You should probably make your own A* class.
		//2. You will need to make this function remember the current progress if it preemptively exits.
		//3. Return "true" if the path is complete, otherwise "false".
		///////////////////////////////////////////////////////////////////////////////////////////////////

		//Check if it is a new request and if it is not in a single step mode
		if (!m_singleStep || newRequest)
		{
			//Check if the straigline mode is on
			if (m_straightline)
			{
				//Get the current row and column
				int curR, curC;
				D3DXVECTOR3 cur = m_owner->GetBody().GetPos();
				g_terrain.GetRowColumn(&cur, &curR, &curC);

				//Compute the minimum and maximum row
				int minR = min(r, curR);
				int maxR = max(r, curR);

				//Compute the minimum and maximum column
				int minC = min(c, curC);
				int maxC = max(c, curC);

				//Variable to check if there is a wall
				bool isWall = false;

				//Go through the rows
				for (int i = minR; i <= maxR; i++)
				{
					//Go through the columns
					for (int j = minC; j <= maxC; j++)
					{
						//Check if there is a wall
						if (g_terrain.IsWall(i, j))
						{
							//Set the variable to true
							isWall = true;
							break;
						}
					}
				}

				//If we didnt find any wall
				if (!isWall)
				{
					//Clear the waypointslist
					m_waypointList.clear();

					//Set the color to blue
					g_terrain.SetColor(curR, curC, DEBUG_COLOR_BLUE);

					//Push the current position and the goal to the final list
					m_waypointList.push_back(cur);
					m_waypointList.push_back(m_goal);

					//Return path found
					return true;
				}

			}

			//Get the current row and column
			int curR, curC;
			D3DXVECTOR3 cur = m_owner->GetBody().GetPos();
			g_terrain.GetRowColumn(&cur, &curR, &curC);

			//Clear all list containers
			m_waypointList.clear();
			g_aStar.m_openList.clear();
			g_aStar.m_closedList.clear();
			g_aStar.m_wayPointListVector.clear();

			//Compute the start node
			A_Star_Node startnode = { 0.0f, 0.0f, {(float)curR, (float)curC }, { -1.0f, -1.0f } };

			//Push the start node onto the openlist
			g_aStar.m_openList.insert({ startnode.m_nodePos, startnode });
		}

		//Check if the list is not empty
		while (!g_aStar.m_openList.empty())
		{
			//Save the biggest index and cost
			A_Star_Node cheapestNode;
			float cheapest_cost = FLT_MAX;


			//Go through the openlist vector
			for (auto & it : g_aStar.m_openList)
			{
				float currentCost = it.second.m_cost;

				//Check if the current value is the cheapest
				if (currentCost < cheapest_cost)
				{
					//Save the new cheapest value and the cost
					cheapestNode = it.second;
					cheapest_cost = currentCost;
				}
			}

			//Check if the cheapest node is equal to the the goal node
			if (cheapestNode.m_nodePos.x == r &&
				cheapestNode.m_nodePos.y == c)
			{
				//Variable to check if points are o nthe list
				bool ordered = false;
				
				//Store the value of the parent as the cheapest node
				D3DXVECTOR2 parent = cheapestNode.m_parentNode;

				//Add the current node to the waypointvector
				g_aStar.m_wayPointListVector.push_back(cheapestNode.m_nodePos);
		
				//Check if the parent node values are -1, so we now that we are in the end
				if (cheapestNode.m_parentNode.x == -1 && cheapestNode.m_parentNode.y == -1)
					ordered = true;

				//Loop until all values are on the list
				while(!ordered)
				{
					auto it = g_aStar.m_closedList.find(parent);

					auto itCloseBegin = g_aStar.m_closedList.begin();
					auto itCloseEnd = g_aStar.m_closedList.end(); 


					//Set the new parent
					parent = it->second.m_parentNode;

					//Push the current node
					g_aStar.m_wayPointListVector.push_back(it->second.m_nodePos);

					//Set the color to yellow
					g_terrain.SetColor((int)it->second.m_nodePos.x, (int)it->second.m_nodePos.y, DEBUG_COLOR_YELLOW);

					//If the cirrent values of the paren are -1,we have finished
					if (parent.x == -1.0f && parent.y == -1.0f)
						ordered = true;
					
				}

				//Reverse the vector for the poitns to be ordered
				std::reverse(g_aStar.m_wayPointListVector.begin(), g_aStar.m_wayPointListVector.end());

				//Check if rubberband is on
				if (m_rubberband)
				{
					//Go through the points 3 by 3
					for (int i = 0; i < (int)g_aStar.m_wayPointListVector.size()-2; i++)
					{
						//Select the minimum row
						int minR = (int)min(g_aStar.m_wayPointListVector[i].x, g_aStar.m_wayPointListVector[i+1].x);
						minR = (int)min(minR, g_aStar.m_wayPointListVector[i + 2].x);

						//Select the maximum row
						int maxR = (int)max(g_aStar.m_wayPointListVector[i].x, g_aStar.m_wayPointListVector[i + 1].x);
						maxR = (int)max(maxR, g_aStar.m_wayPointListVector[i + 2].x);

						//Select the minimum column
						int minC = (int)min(g_aStar.m_wayPointListVector[i].y, g_aStar.m_wayPointListVector[i + 1].y);
						minC = (int)min(minC, g_aStar.m_wayPointListVector[i + 2].y);

						//Select the maximum column
						int maxC = (int)max(g_aStar.m_wayPointListVector[i].y, g_aStar.m_wayPointListVector[i + 1].y);
						maxC = (int)max(maxC, g_aStar.m_wayPointListVector[i + 2].y);

						//Variable to check if there is a wall
						bool isWall = false;

						//Go through the rows
						for (int k = minR; k <= maxR; k++)
						{
							//Go through the columns
							for (int j = minC; j <= maxC; j++)
							{
								//Check if there is a wall
								if (g_terrain.IsWall(k, j))
								{
									//Set the variable to true
									isWall = true;
									break;
								}
							}
						}

						//If there are no walls
						if (!isWall)
						{
							//Erase the current point from the list 
							g_aStar.m_wayPointListVector.erase(g_aStar.m_wayPointListVector.begin() + i + 1);

							//Do not iterate
							i--;
						}		
					}
				}

				//Check if smoothless is activated
				if (m_smooth)
				{
					//Compute the size 
					float size = 1.0f / g_terrain.GetWidth() * 1.5f; 

					//Vector to store the smooth points
					std::vector<D3DXVECTOR3> smoothPoints;

					//Go through the waypoint list vector
					for(int i = 0; i < (int)g_aStar.m_wayPointListVector.size(); i++)
						smoothPoints.push_back(g_terrain.GetCoordinates((int)g_aStar.m_wayPointListVector[i].x, (int)g_aStar.m_wayPointListVector[i].y));

					//Go thorugh the smoothpoints
					for(int i = 0; i < (int)smoothPoints.size()-1; i++)
					{
						//Compute the distance between the two next adjacent point
						D3DXVECTOR3 vector = smoothPoints[i + 1] - smoothPoints[i];

						//Compute the lenght of the obtained vector
						float length = D3DXVec3Length(&vector);

						//Check if the current lenght is bigger than the ciomputed default size
						if (length > size)
						{
							//Compute the new point to insert
							D3DXVECTOR3 pointToInsert = smoothPoints[i] + vector/2;

							//Insert the new point in the current position +1
							smoothPoints.insert(smoothPoints.begin() + i + 1, pointToInsert);
							i--;
						}
					}

					//Store the s values of the formula
					std::vector<float> s;
					s.push_back(0.25f);
					s.push_back(0.5f);
					s.push_back(0.75f);

					//Create the 4 points
					D3DXVECTOR3 point_1;
					D3DXVECTOR3 point_2;
					D3DXVECTOR3 point_3;
					D3DXVECTOR3 point_4;

					//Check if the size is 1
					if (smoothPoints.size() == 1)
					{
						//Push the first point
						m_waypointList.push_back(smoothPoints[0]);
					}

					//Check if the size is 2
					else if (smoothPoints.size() == 2)
					{
						//Push the first point
						m_waypointList.push_back(smoothPoints[0]);

						//Set the point values
						point_1 = smoothPoints[0];
						point_2 = smoothPoints[0];
						point_3 = smoothPoints[1];
						point_4 = smoothPoints[1];

						//Variable to store the output point
						D3DXVECTOR3 pOut;

						//Go through the s values
						for (int j = 0; j < (int)s.size(); j++)
						{
							//Compute catmull rom
							D3DXVec3CatmullRom(&pOut, &point_1, &point_2, &point_3, &point_4, s[j]);

							//Push the point to the waypoint list
							m_waypointList.push_back(pOut);
						}

						//Push the last point
						m_waypointList.push_back(smoothPoints[1]);
					}

					else
					{ 
						//Go through the smoothpoints
						for (int i = 0; i < (int)smoothPoints.size()-1; i++)
						{
							//Push the first point
							m_waypointList.push_back(smoothPoints[i]);

							//Value for the first point
							if (i == 0)
							{
								//Set the point values
								point_1 = smoothPoints[i];
								point_2 = smoothPoints[i];
								point_3 = smoothPoints[i+1];
								point_4 = smoothPoints[i+2];
							}

							//Value for the last point
							else if (i == smoothPoints.size() - 2)
							{
								//Set the point values
								point_1 = smoothPoints[i-1];
								point_2 = smoothPoints[i];
								point_3 = smoothPoints[i + 1];
								point_4 = smoothPoints[i + 1];
							}

							//Intermediate points
							else
							{
								//Set the point values
								point_1 = smoothPoints[i - 1];
								point_2 = smoothPoints[i];
								point_3 = smoothPoints[i + 1];
								point_4 = smoothPoints[i + 2];
							}

							//Variable to store the output point
							D3DXVECTOR3 pOut;

							//Go through the s values
							for (int j = 0; j < (int)s.size(); j++)
							{
								//Compute catmull rom
								D3DXVec3CatmullRom(&pOut, &point_1, &point_2, &point_3, &point_4, s[j]);

								//Push the point to the waypoint list
								m_waypointList.push_back(pOut);
							}
						}
						//Push the last point
						m_waypointList.push_back(smoothPoints[smoothPoints.size()-1]);
					}	
				}

				else
				{
					//Go thrugh the waypoint vectors
					for (int i = 0; i < (int)g_aStar.m_wayPointListVector.size(); i++)
					{
						//Push the point to the waypoint list
						m_waypointList.push_back(g_terrain.GetCoordinates((int)g_aStar.m_wayPointListVector[i].x, (int)g_aStar.m_wayPointListVector[i].y));
					}
				}


				//Go throught the openlist
				for (auto & it : g_aStar.m_openList)
				{
					//Draw the terrain in blue
					g_terrain.SetColor((int)it.second.m_nodePos.x, (int)it.second.m_nodePos.y, DEBUG_COLOR_BLUE);
				}

				for (auto& it : g_aStar.m_closedList)
				{
					//Draw the terrain in blue
					g_terrain.SetColor((int)it.second.m_nodePos.x, (int)it.second.m_nodePos.y, DEBUG_COLOR_YELLOW);
				}

				//Clear the containers
				g_aStar.m_openList.clear();
				g_aStar.m_closedList.clear();
				g_aStar.m_wayPointListVector.clear();


				//Path found
				return true;
			}


			//Remove the cheapest value from the openlist
			auto erasePosition = g_aStar.m_openList.find(cheapestNode.m_nodePos);
			g_aStar.m_openList.erase(erasePosition);

			//Compute the neighbour nodes values
			std::vector<D3DXVECTOR2> neighbourNodes;

			//Varibles to check if there are walls in the N, E, S, W coordinates
			bool Wall1 = false;
			bool Wall2 = false;
			bool Wall3 = false;
			bool Wall4 = false;

			//Check if there are walls and push the nreigbour node
			if (!g_terrain.IsWall((int)cheapestNode.m_nodePos.x + 1, (int)cheapestNode.m_nodePos.y))
			{
				Wall1 = true;
				neighbourNodes.push_back({ cheapestNode.m_nodePos.x + 1, cheapestNode.m_nodePos.y });
			}

			if (!g_terrain.IsWall((int)cheapestNode.m_nodePos.x, (int)cheapestNode.m_nodePos.y - 1))
			{
				Wall2 = true;
				neighbourNodes.push_back({ cheapestNode.m_nodePos.x, cheapestNode.m_nodePos.y - 1 });
			}
			
			if (!g_terrain.IsWall((int)cheapestNode.m_nodePos.x - 1, (int)cheapestNode.m_nodePos.y))
			{
				Wall3 = true;
				neighbourNodes.push_back({ cheapestNode.m_nodePos.x - 1, cheapestNode.m_nodePos.y });
			}
			
			if (!g_terrain.IsWall((int)cheapestNode.m_nodePos.x, (int)cheapestNode.m_nodePos.y + 1))
			{
				Wall4 = true;
				neighbourNodes.push_back({ cheapestNode.m_nodePos.x, cheapestNode.m_nodePos.y + 1 });
			}

			//Check for diagonals and if its able to push them
			if(Wall1 && Wall4)
				if(!g_terrain.IsWall((int)cheapestNode.m_nodePos.x + 1, (int)cheapestNode.m_nodePos.y + 1))
					neighbourNodes.push_back({cheapestNode.m_nodePos.x + 1, cheapestNode.m_nodePos.y + 1}); //Diagonal

			if (Wall1 && Wall2)
				if (!g_terrain.IsWall((int)cheapestNode.m_nodePos.x + 1, (int)cheapestNode.m_nodePos.y - 1))
					neighbourNodes.push_back({cheapestNode.m_nodePos.x + 1, cheapestNode.m_nodePos.y - 1}); //Diagonal		
			
			if (Wall2 && Wall3)
				if (!g_terrain.IsWall((int)cheapestNode.m_nodePos.x - 1, (int)cheapestNode.m_nodePos.y - 1))
					neighbourNodes.push_back({cheapestNode.m_nodePos.x - 1, cheapestNode.m_nodePos.y - 1}); //Diagonal
			
			if (Wall4 && Wall3)
				if (!g_terrain.IsWall((int)cheapestNode.m_nodePos.x - 1, (int)cheapestNode.m_nodePos.y + 1))
					neighbourNodes.push_back({cheapestNode.m_nodePos.x - 1, cheapestNode.m_nodePos.y + 1}); //Diagonal
			

			//Go through the neighbours
			for (unsigned i = 0; i < neighbourNodes.size(); i++)
			{
				//Compute the current neighbour cost
				float heuristicCost = g_aStar.ComputeHeuristicCost(neighbourNodes[i], {(float)r,(float)c}, m_heuristicCalc);
				float givenCost = g_aStar.ComputeGivenCost(cheapestNode, neighbourNodes[i]);

				//Favor paths with lower visibility/exposure.
				if (m_aStarUsesAnalysis)
					givenCost += g_terrain.GetInfluenceMapValue((int)neighbourNodes[i].x, (int)neighbourNodes[i].y) * 20.0f;

				float cost = givenCost + (heuristicCost * m_heuristicWeight);

				auto itOpenBegin = g_aStar.m_openList.begin();
				auto itOpenEnd= g_aStar.m_openList.end();

				auto itCloseBegin = g_aStar.m_closedList.begin();
				auto itCloseEnd = g_aStar.m_closedList.end();

				//Find the current neighbour node in the open and closed list
				auto openListIterator = g_aStar.m_openList.find(neighbourNodes[i]);
				auto closedListIterator = g_aStar.m_closedList.find(neighbourNodes[i]);

				//Variables to check if there is need to be pushed or not founded node
				bool push = false;
				bool notFound = false;

				//If the node is not in the open and in the closed list
				if (openListIterator == g_aStar.m_openList.end() && closedListIterator == g_aStar.m_closedList.end())
				{
					//Set to true as the node was not found
					notFound = true;
				}

				//If the node is in the open or in the open list and if the new cost is cheaper
				if ((openListIterator != g_aStar.m_openList.end())  && (cost < openListIterator->second.m_cost))
				{
					//Set to true as we need to push the node
					push = true;

					//Remove the node from the opened list
					g_aStar.m_openList.erase(openListIterator);
				}

				//If the node is in the open or in the closed list and if the new cost is cheaper
				if ((closedListIterator != g_aStar.m_closedList.end())  && (cost < closedListIterator->second.m_cost))
				{
					push = true;

					//Remove the node from the closed list
					g_aStar.m_closedList.erase(closedListIterator);
				}

				//If we need to push or the node is not found
				if (push || notFound)
				{
					//Add the new node to the open list
					g_aStar.m_openList.insert({ neighbourNodes[i] ,{ cost, givenCost, neighbourNodes[i], cheapestNode.m_nodePos } });
				}

			}

			//Insert the parent node in teh closed list
			g_aStar.m_closedList.insert({ cheapestNode.m_nodePos ,cheapestNode });

			//Check if we need to perform single step
			if (m_singleStep)
			{

				//Go throught the openlist
				for (auto& it : g_aStar.m_openList)
				{
					//Draw the terrain in blue
					g_terrain.SetColor((int)it.second.m_nodePos.x, (int)it.second.m_nodePos.y, DEBUG_COLOR_BLUE);
				}

				//Go throught the closelist
				for (auto& it : g_aStar.m_closedList)
				{
					//Draw the terrain in blue
					g_terrain.SetColor((int)it.second.m_nodePos.x, (int)it.second.m_nodePos.y, DEBUG_COLOR_YELLOW);
				}

				return false;
			}

		}
		
		//Push the current body position
		m_waypointList.push_back(m_owner->GetBody().GetPos());

		//Go throught the openlist
		for (auto& it : g_aStar.m_openList)
		{
			//Draw the terrain in blue
			g_terrain.SetColor((int)it.second.m_nodePos.x, (int)it.second.m_nodePos.y, DEBUG_COLOR_BLUE);
		}

		for (auto& it : g_aStar.m_closedList)
		{
			//Draw the terrain in blue
			g_terrain.SetColor((int)it.second.m_nodePos.x, (int)it.second.m_nodePos.y, DEBUG_COLOR_YELLOW);
		}

		//Paht found
		return true;

	}
	else
	{	
		//Randomly meander toward goal (might get stuck at wall)
		int curR, curC;
		D3DXVECTOR3 cur = m_owner->GetBody().GetPos();
		g_terrain.GetRowColumn( &cur, &curR, &curC );

		m_waypointList.clear();
		m_waypointList.push_back( cur );

		int countdown = 100;
		while( curR != r || curC != c )
		{
			if( countdown-- < 0 ) { break; }

			if( curC == c || (curR != r && rand()%2 == 0) )
			{	//Go in row direction
				int last = curR;
				if( curR < r ) { curR++; }
				else { curR--; }

				if( g_terrain.IsWall( curR, curC ) )
				{
					curR = last;
					continue;
				}
			}
			else
			{	//Go in column direction
				int last = curC;
				if( curC < c ) { curC++; }
				else { curC--; }

				if( g_terrain.IsWall( curR, curC ) )
				{
					curC = last;
					continue;
				}
			}

			D3DXVECTOR3 spot = g_terrain.GetCoordinates( curR, curC );
			m_waypointList.push_back( spot );
			g_terrain.SetColor( curR, curC, DEBUG_COLOR_YELLOW );
		}
		return true;
	}
}
