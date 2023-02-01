// Author: Chi-Hao Kuo
// Updated: 12/25/2015

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

void Enemy::FieldOfView(float angle, float closeDistance, float occupancyValue)
{
	// TODO: Implement this for the Occupancy Map project.

	// Parameters:
	//   angle - view cone angle in degree
	//   closeDistance - if the tile is within this distance, no need to check angle
	//   occupancyValue - if the tile is in FOV, apply this value

	// first, clear out old occupancyValue from previous frame
	// (mark m_terrainInfluenceMap[r][c] as zero if the old value is negative

	// For every square on the terrain that is within the field of view cone
	// by the enemy square, mark it with occupancyValue.

	// If the tile is close enough to the enemy (less than closeDistance),
	// you only check if it's visible to the enemy.
	// Otherwise you must also consider the direction the enemy is facing:

	// Get enemy's position and direction
	//   D3DXVECTOR3 pos = m_owner->GetBody().GetPos();
	//   D3DXVECTOR3 dir = m_owner->GetBody().GetDir();

	// Give the enemyDir a field of view a tad greater than the angle:
	//   D3DXVECTOR2 enemyDir = D3DXVECTOR2(dir.x, dir.z);

	// Two grid squares are visible to each other if a line between 
	// their centerpoints doesn't intersect the four boundary lines
	// of every walled grid square. Put this code in IsClearPath().

	if (!g_blackboard.GetTerrainAnalysisFlag() ||
		(g_blackboard.GetTerrainAnalysisType() != TerrainAnalysis_HideAndSeek))
		return;


	// WRITE YOUR CODE HERE

		//Variable to store the width of the terrain
	int width = g_terrain.GetWidth();

	//Go through the terrain rows
	for (int i = 0; i < width; i++)
	{
		//Go through the terrain columns
		for (int j = 0; j < width; j++)
		{
			//Check if the influence values is negative and set the value to 0
			if (g_terrain.GetInfluenceMapValue(i, j) < 0.0f)
				g_terrain.SetInfluenceMapValue(i, j, 0.0f);
		}
	}

	//Go through the terrain rows
	for (int i = 0; i < width; i++)
	{
		//Go through the terrain columns
		for (int j = 0; j < width; j++)
		{
			//Compute the position and direction of the enemy
			D3DXVECTOR3 pos = m_owner->GetBody().GetPos();
			D3DXVECTOR3 dir = m_owner->GetBody().GetDir();

			//Compute the distance from the tile to the enemy
			float distanceTileEnemy = D3DXVec3Length(&((g_terrain.GetCoordinates(i, j) - pos)* (float)width));

			//Check if the current distance is smaller than the close one
			if (distanceTileEnemy < closeDistance)
			{
				//Take the row and the column of the enemy
				int row, col;
				g_terrain.GetRowColumn(&pos, &row, &col);

				//Check if the path is clear set the value to occupancyValue
				if (g_terrain.IsClearPath(i, j, row, col))
					g_terrain.SetInfluenceMapValue(i, j, occupancyValue);
			}

			else
			{
				D3DXVECTOR2 enemyDir;
				D3DXVec2Normalize(&enemyDir, &D3DXVECTOR2(dir.x, dir.z));
	
				//Compute the vector between the enemy and the goal and normalize it
				D3DXVECTOR3 enemyGoalDir;
				D3DXVec3Normalize(&enemyGoalDir, &(g_terrain.GetCoordinates(i, j) - pos));
	
				//Comput the dot product
				float dotProduct = enemyDir.x * enemyGoalDir.x + enemyDir.y * enemyGoalDir.z;
	
				//Compute the lenght of the two vectors
				float enemyDirLength = D3DXVec2Length(&enemyDir);
				float enemyGoalDirLength = D3DXVec3Length(&enemyGoalDir);
	
				//Variable to compute the angle
				float newAngle = 0.0f;
	
				//Check if the divisor to get the angle is biger than 0.0f and compute the angle
				if ((enemyDirLength * enemyGoalDirLength) > 0.0f)
				{
					//Compute the value inside the cosine
					float value = dotProduct / (enemyDirLength * enemyGoalDirLength);
					
					//Check if the computed value is bigger than 1
					if (value > 1.0f)
						value = 1.0f;

					//Compute the cosine
					newAngle = acosf(value);
				}
					
				//Check if the new angle is smaller than given angle
				if (newAngle <= angle * D3DX_PI / (2.0f * 180.0f))
				{
					int row, col;
					g_terrain.GetRowColumn(&pos, &row, &col);
	
					//Check if the path is clear set the value to occupancyValue
					if (g_terrain.IsClearPath(i, j, row, col))
						g_terrain.SetInfluenceMapValue(i, j, occupancyValue);
				}
			}
		}
	}
}

bool Enemy::FindPlayer(void)
{
	// TODO: Implement this for the Occupancy Map project.

	// Check if the player's tile has negative value (FOV cone)
	// Return true if player is within field cone of view of enemy
	// Return false otherwise
	
	// You need to also set new goal to player's position:
	//   ChangeGoal(row_player, col_player);


	// WRITE YOUR CODE HERE

	//Get player row and col
	int playerRow = g_blackboard.GetRowPlayer();
	int playerCol = g_blackboard.GetColPlayer();

	//Check if the player's tile has negative value 
	if (g_terrain.GetInfluenceMapValue(playerRow, playerCol) < 0.0f)
	{
		//Set new goal to player's position
		ChangeGoal(playerRow, playerCol);
		return true;
	}

	return false;
}

bool Enemy::SeekPlayer(void)
{
	// TODO: Implement this for the Occupancy Map project.

	// Find the tile with 1.0 occupancy value, and set it as 
	// the new goal:
	//   ChangeGoal(row, col);
	// If multiple tiles with 1.0 occupancy, pick the closest
	// tile to the enemy

	// Return false if no tile is found


	// WRITE YOUR CODE HERE

	//Variable to store the width of the terrain
	int width = g_terrain.GetWidth();

	//Value to store the minimum distance
	float minDistance = FLT_MAX;

	//Variable to store the row
	int row = 0;
	int col = 0;

	//Variable to set if the tile was found
	bool found = false;

	//Go through the terrain rows
	for (int i = 0; i < width; i++)
	{
		//Go through the terrain columns
		for (int j = 0; j < width; j++)
		{
			if (!g_terrain.IsWall(i, j))
			{
				//Check if the current tile value is 1.0f
				if (g_terrain.GetInfluenceMapValue(i, j) == 1.0f)
				{
					//Save the position of the enemy
					D3DXVECTOR3 pos = m_owner->GetBody().GetPos();

					//Compute the distance from tiny
					float currentDistance = D3DXVec3Length(&((pos - g_terrain.GetCoordinates(i, j) * (float)width)));

					//Check if the current distance is smaller than the saved one
					if (minDistance > currentDistance)
					{
						//Store the new minimum ditance
						minDistance = currentDistance;

						//Store the new row and columns
						row = i;
						col = j;

						//Tile found
						found = true;
					}
				}
			}
		}
	}

	//If the value 1.0 is found
	if (found)
	{
		//Change the goal
		ChangeGoal(row, col);

		//Tile found
		return true;
	}

	//No tile found
	return false;
}
