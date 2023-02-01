/**
 * @file nodes.hpp
 * @author Gaizka Calvo (gaizka.c@digipen.edu)
 * @date 21/09/2020
 *
 * @copyright Copyright (c) 2020
 * @brief Class that we will use to store the BV
 */
#pragma once
#include "Debug/geometry.hpp"

namespace cs350
{
	enum class node_type
	{
		LEAF,
		NODE
	};

	struct BVH_TD_node
	{
		BVH_TD_node();
		~BVH_TD_node();
		
		std::vector<glm::vec3>* mObject;
		int mNumbrerObj;

		BVH_TD_node* mLeftNode;
		BVH_TD_node* mRightNode;

		aabb mBV;
		bool mShowMesh;

		node_type mType;
	};

	struct BVH_node
	{
		BVH_node();
		~BVH_node();

		std::vector<int> mObject;

		BVH_node* mLeftNode;
		BVH_node* mRightNode;

		aabb mBV;
		bool mShowMesh;

		node_type mType;
	};


	typedef BVH_node* BVH_node_ptr;
}