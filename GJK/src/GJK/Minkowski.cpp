#include "Helpers/pch.hpp"
#include "Minkowski.hpp"
#include "GameObjects/game_object.hpp"
#include <algorithm>


cs350::minkowski::minkowski() : mDebugDraw(false)
{
}

cs350::minkowski::~minkowski()
{
}

void cs350::minkowski::create_minkowski(GameObject* ob1, GameObject* ob2)
{
	if (minkoski_created())
		delete_minkowski();
	mMesh.mMeshData.positions;

	auto m2w1 = ob1->mTransform.GetModelToWorld();
	auto m2w2 = ob2->mTransform.GetModelToWorld();

	for (auto & it1 : ob1->mMod->mMeshData.vertexs)
	{
		for (auto& it2 : ob2->mMod->mMeshData.vertexs)
		{
			mMesh.mMeshData.positions.push_back(glm::vec3( (m2w1 * glm::vec4(it1, 1.0f)) - (m2w2 * glm::vec4(it2, 1.0f))) );
		}
	}

	mMesh.create(mMesh.mMeshData);
}

void cs350::minkowski::delete_minkowski()
{
	mMesh.mMeshData.positions.clear();
	mMesh.clear();
}

bool cs350::minkowski::get_debug_draw() const
{
	return mDebugDraw;
}

void cs350::minkowski::set_debug_draw(bool state)
{
	mDebugDraw = state;
}

bool cs350::minkowski::minkoski_created()
{
	return !mMesh.mMeshData.positions.empty();
}


