#include "Helpers/pch.hpp"
#include "nodes.hpp"

cs350::BVH_node::BVH_node() : mLeftNode(nullptr), mRightNode(nullptr), mShowMesh(false)
{

}

cs350::BVH_node::~BVH_node()
{
}


cs350::BVH_TD_node::BVH_TD_node() : mObject(nullptr), mNumbrerObj(0), mLeftNode(nullptr), mRightNode(nullptr), mShowMesh(false)
{
}

cs350::BVH_TD_node::~BVH_TD_node()
{
}
