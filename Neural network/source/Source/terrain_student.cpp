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


float Terrain::ClosestWall(int row, int col)
{
	// TODO: Helper function for the Terrain Analysis project.

	// For each tile, check the distance with every wall of the map.
	// Return the distance to the closest wall or side.

	// WRITE YOUR CODE HERE

	//Variable to store the width of the terrain
	int width = GetWidth();

	//Variable to store the minimum distance
	float minDistance = FLT_MAX;

	//Compute the distance between the side and the given value
	D3DXVECTOR3 up = (GetCoordinates(row, col) - GetCoordinates(row,-1))* (float)width;
	D3DXVECTOR3 down = (GetCoordinates(row, col) - GetCoordinates(row, width))* (float)width;
	D3DXVECTOR3 right = (GetCoordinates(row, col) - GetCoordinates(-1, col))* (float)width;
	D3DXVECTOR3 left = (GetCoordinates(row, col) - GetCoordinates(width, col)) * (float)width;

	//Compute the lenght of the obtained vector
	float currentDistanceUp = D3DXVec3Length(&up);
	float currentDistanceDown = D3DXVec3Length(&down);
	float currentDistanceRight = D3DXVec3Length(&right);
	float currentDistanceLeft = D3DXVec3Length(&left);

	//Check if  the current distance is smaller than the stored one
	if (currentDistanceUp < minDistance)
	{
		//Store the new minimum distance
		minDistance = currentDistanceUp;
	}

	//Check if  the current distance is smaller than the stored one
	if (currentDistanceDown < minDistance)
	{
		//Store the new minimum distance
		minDistance = currentDistanceDown;
	}

	//Check if  the current distance is smaller than the stored one
	if (currentDistanceRight < minDistance)
	{
		//Store the new minimum distance
		minDistance = currentDistanceRight;
	}

	//Check if  the current distance is smaller than the stored one
	if (currentDistanceLeft < minDistance)
	{
		//Store the new minimum distance
		minDistance = currentDistanceLeft;
	}


	//Go through the terrain rows
	for (int i = 0; i < width; i++)
	{
		//Go through the terrain columns
		for (int j = 0; j < width; j++)
		{
			//Check if the current position is a wall
			if (IsWall(i, j))
			{
				//Compute the distance between the wall and the given value
				D3DXVECTOR3 vector = (GetCoordinates(row,col) - GetCoordinates(i, j)) * (float)width;

				//Compute the lenght of the obtained vector
				float currentDistance = D3DXVec3Length(&vector);

				//Check if  the current distance is smaller than the stored one
				if (currentDistance < minDistance)
				{
					//Store the new minimum distance
					minDistance = currentDistance;
				}
			}
		}
	}
	
	//Return the minimum distance
	return minDistance;	//REPLACE THIS
}

void Terrain::AnalyzeOpennessClosestWall(void)
{
	// TODO: Implement this for the Terrain Analysis project.

	// Mark every square on the terrain (m_terrainInfluenceMap[r][c]) with
	// the value 1/(d*d), where d is the distance to the closest wall in 
	// row/column grid space.
	// Edges of the map count as walls!
	// 
	// WRITE YOUR CODE HERE

	//Variable to store the width of the terrain
	int width = GetWidth();

	//Go through the terrain rows
	for (int i = 0; i < width; i++)
	{
		//Go through the terrain columns
		for (int j = 0; j < width; j++)
		{
			//Get the current closet wall distance
			float distance = ClosestWall(i, j);

			//Check if there is a wall
			if (IsWall(i, j))
			{
				//Set the value of the influence map to 0
				m_terrainInfluenceMap[i][j] = 0.0f;
			}

			//If there isnt a wall
			else
			{
				//Set the value of the influence map to  1/(d*d)
				m_terrainInfluenceMap[i][j] = 1 / (distance * distance);
			}
		}
	}
}

void Terrain::AnalyzeVisibility(void)
{
	// TODO: Implement this for the Terrain Analysis project.

	// Mark every square on the terrain (m_terrainInfluenceMap[r][c]) with
	// the number of grid squares (that are visible to the square in question)
	// divided by 160 (a magic number that looks good). Cap the value at 1.0.

	// Two grid squares are visible to each other if a line between 
	// their centerpoints doesn't intersect the four boundary lines
	// of every walled grid square. Put this code in IsClearPath().


	// WRITE YOUR CODE HERE


	//Variable to store the width of the terrain
	int width = GetWidth();

	//Go through the terrain rows
	for (int i = 0; i < width; i++)
	{
		//Go through the terrain columns
		for (int j = 0; j < width; j++)
		{
			float counter = 0.0f;

			//Go through the terrain rows
			for (int i1 = 0; i1 < width; i1++)
			{
				//Go through the terrain columns
				for (int j1 = 0; j1 < width; j1++)
				{
					//Check for itself
					if (i == i1 && j == j1)
						continue;

					//Check if the path is clear and increase the counter
					if (IsClearPath(i, j, i1, j1))
						counter++;
				}
			}

			//Set the counter divided by the 160 to the influence map value
			m_terrainInfluenceMap[i][j] = counter/160.0f;

			//Cap the valyue to 1
			if (m_terrainInfluenceMap[i][j] > 1.0f)
				m_terrainInfluenceMap[i][j] = 1.0f;
		}
	}

}

void Terrain::AnalyzeVisibleToPlayer(void)
{
	// TODO: Implement this for the Terrain Analysis project.

	// For every square on the terrain (m_terrainInfluenceMap[r][c])
	// that is visible to the player square, mark it with 1.0.
	// For all non 1.0 squares that are visible to, and next to 1.0 squares,
	// mark them with 0.5. Otherwise, the square should be marked with 0.0.

	// Two grid squares are visible to each other if a line between 
	// their centerpoints doesn't intersect the four boundary lines
	// of every walled grid square. Put this code in IsClearPath().

	// WRITE YOUR CODE HERE

	//Variable to store the width of the terrain
	int width = GetWidth();
	
	//Go through the terrain rows
	for (int i = 0; i < width; i++)
	{
		//Go through the terrain columns
		for (int j = 0; j < width; j++)
		{
			//Check if the path is clear set the value to 1.0f
			if (IsClearPath(i, j, m_rPlayer, m_cPlayer))
				m_terrainInfluenceMap[i][j] = 1.0f;

			//if the path is not clear set the value to 0.0f
			else
				m_terrainInfluenceMap[i][j] = 0.0f;
		}
	}

	//Go through the terrain rows
	for (int i = 0; i < width; i++)
	{
		//Go through the terrain columns
		for (int j = 0; j < width; j++)
		{
			//Check if the current grid value is 1.0f
			if (m_terrainInfluenceMap[i][j] == 1.0f)
			{
				//Check for the 8 neighbours value if it is not out of bounce.
				//if that neigbourd grid value is not 1.0f 
				//and if the path between the current and  the neigbour grid
				//Finally set the grif value to 0.5f
				if(!(i + 1 >= m_width || j + 1 >= m_width))
					if(m_terrainInfluenceMap[i+1][j+1]!= 1.0f)
						if (IsClearPath(i + 1, j + 1, i, j))
							m_terrainInfluenceMap[i + 1][j + 1] = 0.5f;

				if(!(i + 1 >= m_width))
					if (m_terrainInfluenceMap[i + 1][j] != 1.0f)
						if (IsClearPath(i + 1, j, i, j))
							m_terrainInfluenceMap[i + 1][j] = 0.5f;

				if(!(j + 1 >= m_width))
					if (m_terrainInfluenceMap[i][j+1] != 1.0f)
						if (IsClearPath(i, j + 1, i, j))
							m_terrainInfluenceMap[i][j + 1] = 0.5f;

				if(!(i-1 < 0 || j-1 < 0))
					if (m_terrainInfluenceMap[i-1][j - 1] != 1.0f)
						if (IsClearPath(i - 1, j - 1, i, j))
							m_terrainInfluenceMap[i - 1][j - 1] = 0.5f;

				if(!(i - 1 < 0))
					if (m_terrainInfluenceMap[i - 1][j] != 1.0f)
						if (IsClearPath(i - 1, j, i, j))
							m_terrainInfluenceMap[i - 1][j] = 0.5f;

				if(!(j - 1 < 0))
					if (m_terrainInfluenceMap[i][j-1] != 1.0f)
						if (IsClearPath(i, j - 1, i, j))
							m_terrainInfluenceMap[i][j - 1] = 0.5f;

				if(!(i - 1 < 0 || j + 1 >= m_width))
					if (m_terrainInfluenceMap[i - 1][j + 1] != 1.0f)
						if (IsClearPath(i - 1, j + 1, i, j))
							m_terrainInfluenceMap[i - 1][j + 1] = 0.5f;

				if(!(i + 1 >= m_width || j - 1 < 0))
					if (m_terrainInfluenceMap[i + 1][j - 1] != 1.0f)
						if (IsClearPath(i + 1, j - 1, i, j))
							m_terrainInfluenceMap[i + 1][j - 1] = 0.5f;
			}
		}
	}
}

void Terrain::AnalyzeSearch(void)
{
	// TODO: Implement this for the Terrain Analysis project.

	// For every square on the terrain (m_terrainInfluenceMap[r][c])
	// that is visible by the player square, mark it with 1.0.
	// Otherwise, don't change the value (because it will get
	// decremented with time in the update call).
	// You must consider the direction the player is facing:
	// D3DXVECTOR2 playerDir = D3DXVECTOR2(m_dirPlayer.x, m_dirPlayer.z)
	// Give the player a field of view a tad greater than 180 degrees.

	// Two grid squares are visible to each other if a line between 
	// their centerpoints doesn't intersect the four boundary lines
	// of every walled grid square. Put this code in IsClearPath().


	// WRITE YOUR CODE HERE

	//Variable to store the width of the terrain
	int width = GetWidth();

	//Compute th player forward vector and normalize
	D3DXVECTOR2 playerDir;
	D3DXVec2Normalize(&playerDir,&D3DXVECTOR2(m_dirPlayer.x, m_dirPlayer.z));

	//Go through the terrain rows
	for (int i = 0; i < width; i++)
	{
		//Go through the terrain columns
		for (int j = 0; j < width; j++)
		{
			//Compute the vector between the player and the goal and normalize it
			D3DXVECTOR3 playerGoalDir;
			D3DXVec3Normalize(&playerGoalDir, &(GetCoordinates(i, j) - GetCoordinates(m_rPlayer, m_cPlayer)));

			//Comput the dot product
			float dotProduct = playerDir.x * playerGoalDir.x + playerDir.y * playerGoalDir.z;

			//Compute the lenght of the two vectors
			float playerDirLength = D3DXVec2Length(&playerDir);
			float playerGoalDirLength = D3DXVec3Length(&playerGoalDir);

			//Variable to compute the angle
			float angle = 0.0f;

			//Check if the divisor to get the angle is biger than 0.0f and compute the angle
			if ((playerDirLength * playerGoalDirLength) > 0.0f)
				angle = acosf(dotProduct / (playerDirLength * playerGoalDirLength));

			//Check if the angle is smaller than 90
			if (angle <= D3DX_PI /2.0f)
			{
				//Check if the path is clear set the value to 1.0f
				if (IsClearPath(i, j, m_rPlayer, m_cPlayer))
					m_terrainInfluenceMap[i][j] = 1.0f;
			}
		}
	}
}

bool Terrain::IsClearPath(int r0, int c0, int r1, int c1)
{
	// TODO: Implement this for the Terrain Analysis project.

	// Two grid squares (r0,c0) and (r1,c1) are visible to each other 
	// if a line between their centerpoints doesn't intersect the four 
	// boundary lines of every walled grid square. Make use of the
	// function LineIntersect(). You should also puff out the four
	// boundary lines by a very tiny bit so that a diagonal line passing
	// by the corner will intersect it.

	// WRITE YOUR CODE HERE

	//Variable to store the width of the terrain
	int width = GetWidth();
	
	//Compute the two point of the segment
	D3DXVECTOR3 point1 = GetCoordinates(r0, c0);
	D3DXVECTOR3 point2 = GetCoordinates(r1, c1);

	//Compute the minimum and maximum row
	int minR = min(r0, r1);
	int maxR = max(r0, r1);

	//Compute the minimum and maximum column
	int minC = min(c0, c1);
	int maxC = max(c0, c1);

	//Go through the terrain rows
	for (int i = minR; i <= maxR; i++)
	{
		//Go through the terrain columns
		for (int j = minC; j <= maxC; j++)
		{
			//Check if there is a wall
			if (IsWall(i, j))
			{
				//Get the coordinate values of the wall
				D3DXVECTOR3 coordinateWall = GetCoordinates(i,j);

				//Compute the sides of the wall
				D3DXVECTOR3 upWall1 =    { coordinateWall.x - (1.0f / width / 2.0f) - 0.0001f, 0.0f,  coordinateWall.z + (1.0f / width / 2.0f) + 0.0001f };
				D3DXVECTOR3 upWall2 =    { coordinateWall.x + (1.0f / width / 2.0f) + 0.0001f, 0.0f,  coordinateWall.z + (1.0f / width / 2.0f) + 0.0001f };
				D3DXVECTOR3 downWall1 =  { coordinateWall.x - (1.0f / width / 2.0f) - 0.0001f, 0.0f,  coordinateWall.z - (1.0f / width / 2.0f) - 0.0001f };
				D3DXVECTOR3 downWall2 =  { coordinateWall.x + (1.0f / width / 2.0f) + 0.0001f, 0.0f,  coordinateWall.z - (1.0f / width / 2.0f) - 0.0001f };
				D3DXVECTOR3 rightWall1 = { coordinateWall.x + (1.0f / width / 2.0f) + 0.0001f, 0.0f,  coordinateWall.z + (1.0f / width / 2.0f) + 0.0001f };
				D3DXVECTOR3 rightWall2 = { coordinateWall.x + (1.0f / width / 2.0f) + 0.0001f, 0.0f,  coordinateWall.z - (1.0f / width / 2.0f) - 0.0001f };
				D3DXVECTOR3 leftWall1 =  { coordinateWall.x - (1.0f / width / 2.0f) - 0.0001f, 0.0f,  coordinateWall.z + (1.0f / width / 2.0f) + 0.0001f };
				D3DXVECTOR3 leftWall2 =  { coordinateWall.x - (1.0f / width / 2.0f) - 0.0001f, 0.0f,  coordinateWall.z - (1.0f / width / 2.0f) - 0.0001f };

				//Check if any wall intersects with the segment and return false
				if (LineIntersect(upWall1.x, upWall1.z, upWall2.x, upWall2.z, point1.x, point1.z, point2.x, point2.z))
					return false;

				if (LineIntersect(downWall1.x, downWall1.z, downWall2.x, downWall2.z, point1.x, point1.z, point2.x, point2.z))
					return false;

				if (LineIntersect(rightWall1.x, rightWall1.z, rightWall2.x, rightWall2.z, point1.x, point1.z, point2.x, point2.z))
					return false;

				if (LineIntersect(leftWall1.x, leftWall1.z, leftWall2.x, leftWall2.z, point1.x, point1.z, point2.x, point2.z))
					return false;
			}
		}
	}

	//Return true when there are no intersections
	return true;	//REPLACE THIS
}

void Terrain::Propagation(float decay, float growing, bool computeNegativeInfluence)
{
	// TODO: Implement this for the Occupancy Map project.

	// computeNegativeInfluence flag is true if we need to handle two agents
	// (have both positive and negative influence)
	// computeNegativeInfluence flag is false if we only deal with positive
	// influence (we should ignore all negative influence)

	// Pseudo code:
	//
	// For each tile on the map
	//
	//   Get the influence value of each neighbor after decay
	//   Then keep the decayed influence WITH THE HIGHEST ABSOLUTE.
	//
	//   Apply linear interpolation to the influence value of the tile, 
	//   and the maximum decayed influence value from all neighbors, with growing 
	//   factor as coefficient
	//
	//   Store the result to the temp layer
	//
	// Store influence value from temp layer

	// WRITE YOUR CODE HERE

	//Variable to store the width of the terrain
	int width = GetWidth();

	//"2D vector" to store the values of the temp layer
	std::vector<std::vector<float>> tempLayer;

	//Resize the vectors
	tempLayer.resize(width);
	for (int i = 0; i < width; i++)
		tempLayer[i].resize(width);

	//Go through the terrain rows
	for (int i = 0; i < width; i++)
	{
		//Go through the terrain columns
		for (int j = 0; j < width; j++)
		{
			//Check if the compute negative is false and if the value is negative
			if (!computeNegativeInfluence && m_terrainInfluenceMap[i][j] < 0.0f)
			{
				//Set the temp layer value
				tempLayer[i][j] = m_terrainInfluenceMap[i][j];
				continue;
			}

			//Check if its a wall
			if (IsWall(i, j))
			{
				//Set the temp value to 0
				tempLayer[i][j] = 0.0f;
				continue;
			}

			//Compute the neighbour nodes values
			std::vector<D3DXVECTOR2> neighbourNodes;

			//Check if there are walls and push the nreigbour node
			if (!IsWall(i + 1, j))
				neighbourNodes.push_back({ (float)i + 1, (float)j });

			if (!IsWall(i, j - 1))
				neighbourNodes.push_back({ (float)i, (float)j - 1 });

			if (!IsWall(i - 1, j))
				neighbourNodes.push_back({ (float)i - 1, (float)j });

			if (!IsWall(i, j + 1))
				neighbourNodes.push_back({ (float)i, (float)j + 1 });

			if (!IsWall(i + 1, j + 1))
				neighbourNodes.push_back({ (float)i + 1, (float)j + 1 });

			if (!IsWall(i + 1, j - 1))
				neighbourNodes.push_back({ (float)i + 1, (float)j - 1 });

			if (!IsWall(i - 1, j - 1))
				neighbourNodes.push_back({ (float)i - 1, (float)j - 1 });

			if (!IsWall(i - 1, j + 1))
				neighbourNodes.push_back({ (float)i - 1, (float)j + 1 });

			//Store the size of the neighbours
			int neighbourSize = neighbourNodes.size();

			//Store the maximum new influence value
			float maxInfluence = 0.0f;

			//Go through the neighbpurs
			for (int k = 0; k < neighbourSize; k++)
			{
				//Negative values
				if (computeNegativeInfluence && m_terrainInfluenceMap[(int)neighbourNodes[k].x][(int)neighbourNodes[k].y] < 0.0f)
				{
					//Compute the x and y differences from the neighbourNodes node to the parent node
					float xDiff = std::fabs(neighbourNodes[k].x - i);
					float yDiff = std::fabs(neighbourNodes[k].y - j);

					//Variable to store the distance
					float distance = 0.0f;

					// Normal position nodes
					if (xDiff == 0 || yDiff == 0)
						distance = 1.0f;

					//Diagonal positions nodes
					else
						distance = sqrtf(2);

					//Compute the new influence
					float newInfluence = m_terrainInfluenceMap[(int)neighbourNodes[k].x][(int)neighbourNodes[k].y] * exp(-1 * distance * decay);

					//Check if the current new influence is bigger thant the stored one
					if (abs(maxInfluence) < abs(newInfluence))
						maxInfluence = newInfluence;
				}

				//Positive values
				if (m_terrainInfluenceMap[(int)neighbourNodes[k].x][(int)neighbourNodes[k].y] > 0.0f)
				{
					//Compute the x and y differences from the neighbourNodes node to the parent node
					float xDiff = std::fabs(neighbourNodes[k].x - i);
					float yDiff = std::fabs(neighbourNodes[k].y - j);

					//Variable to store the distance
					float distance = 0.0f;

					// Normal position nodes
					if (xDiff == 0 || yDiff == 0)
						distance = 1.0f;

					//Diagonal positions nodes
					else
						distance = sqrtf(2);

					//Compute the new influence
					float newInfluence = m_terrainInfluenceMap[(int)neighbourNodes[k].x][(int)neighbourNodes[k].y] * exp(-1 * distance * decay);

					//Check if the current new influence is bigger thant the stored one
					if (abs(maxInfluence) < newInfluence)
						maxInfluence = newInfluence;
				}
			}

			//Compute the linear interpolation
			tempLayer[i][j] = (1 - growing) * m_terrainInfluenceMap[i][j] + growing * maxInfluence;
		}
	}

	//Go through the terrain rows
	for (int i = 0; i < width; i++)
	{
		//Go through the terrain columns
		for (int j = 0; j < width; j++)
		{
			//Save the computed layer on the actual influence map
			m_terrainInfluenceMap[i][j] = tempLayer[i][j];
		}
	}
}

void Terrain::NormalizeOccupancyMap(bool computeNegativeInfluence)
{
	// TODO: Implement this for the Occupancy Map project.

	// divide all tiles with maximum influence value, so the range of the
	// influence is kept in [0,1]
	// if we need to handle negative influence value, divide all positive
	// tiles with maximum influence value, and all negative tiles with
	// minimum influence value * -1, so the range of the influence is kept
	// at [-1,1]
	
	// computeNegativeInfluence flag is true if we need to handle two agents
	// (have both positive and negative influence)
	// computeNegativeInfluence flag is false if we only deal with positive
	// influence, ignore negative influence 

	// WRITE YOUR CODE HERE

	//Variable to store the width of the terrain
	int width = GetWidth();

	//Variable to store the maximum influence value
	float maxInfluenceValue = 0.0f;
	float minInfluenceValue = FLT_MAX;

	//Go through the terrain rows
	for (int i = 0; i < width; i++)
	{
		//Go through the terrain columns
		for (int j = 0; j < width; j++)
		{
			//Store the maximum influence value
			if (maxInfluenceValue < m_terrainInfluenceMap[i][j])
				maxInfluenceValue = m_terrainInfluenceMap[i][j];

			//Store the minimum influence value
			if (m_terrainInfluenceMap[i][j] < minInfluenceValue)
				minInfluenceValue = m_terrainInfluenceMap[i][j];
		}
	}

	//Go through the terrain rows
	for (int i = 0; i < width; i++)
	{
		//Go through the terrain columns
		for (int j = 0; j < width; j++)
		{
			//Negative values
			if(computeNegativeInfluence && m_terrainInfluenceMap[i][j] < 0.0f)
				m_terrainInfluenceMap[i][j] /= -1 * minInfluenceValue;

			//Positive values
			if (m_terrainInfluenceMap[i][j] > 0.0f)
				m_terrainInfluenceMap[i][j] /= maxInfluenceValue; 				
		}
	}
}
