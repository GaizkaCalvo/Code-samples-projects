#include "Helpers/pch.hpp"
#include "BVH/bvh.hpp"

cs350::bvh::bvh() : mMinimum_leaf_obj(0), mDrawBVHTriangles(false), mTDRoot(nullptr), mBURoot(nullptr), mINSRoot(nullptr)
{
}

cs350::bvh::~bvh()
{
}

cs350::BVH_node* cs350::bvh::get_bu_root()
{
	return mBURoot;
}

cs350::BVH_node* cs350::bvh::get_ins_root()
{
	return mINSRoot;
}


void cs350::bvh::bottom_up_bvt_tree(cs350::BVH_node** tree, std::vector<glm::vec3>* objects, int num_obj)
{
	int i, j;
	// Allocate temporary memory for holding node pointers to
	// the current set of active nodes (initially the leaves)
	cs350::BVH_node_ptr* pNodes = new cs350::BVH_node_ptr[num_obj];

	// Form the leaf nodes for the given input objects
	for (i = 0; i < num_obj; i++)
	{
		pNodes[i] = new cs350::BVH_node;
		pNodes[i]->mObject.push_back(i);
		pNodes[i]->mType = cs350::node_type::LEAF;
		pNodes[i]->mBV = compute_bounding_volume(&objects[i], 1);
	}

	// Merge pairs together until just the root object left
	while (num_obj > 1)
	{
		// Find indices of the two "nearest" nodes, based on some criterion
		find_nodes_to_merge(&pNodes[0], num_obj, i, j);
		// Group nodes i and j together under a new internal node
		cs350::BVH_node* pPair = new cs350::BVH_node;
		pPair->mType = cs350::node_type::NODE;
		pPair->mLeftNode = pNodes[i];
		pPair->mRightNode = pNodes[j];
		//copy all the triangles
		pPair->mObject.insert(pPair->mObject.begin(), pNodes[i]->mObject.begin(), pNodes[i]->mObject.end());
		pPair->mObject.insert(pPair->mObject.end(), pNodes[j]->mObject.begin(), pNodes[j]->mObject.end());
		// Compute a bounding volume for the two nodes
		//pPair->mBV = compute_bounding_volume(pNodes[i]->mObject, pNodes[j]->mObject);
		pPair->mBV = compute_bounding_volume(pNodes[i]->mBV, pNodes[j]->mBV);

		// Remove the two nodes from the active set and add in the new node.
		// Done by putting new node at index ’min’ and copying last entry to ’max’
		int min = i, max = j;
		if (i > j) min = j, max = i;
		//we put the pair on the minimun value
		pNodes[min] = pPair;
		//we put in the max position the object that should be
		pNodes[max] = pNodes[num_obj - 1];
		num_obj--;
	}

	// Free temporary storage and return root of tree
	mBURoot = pNodes[0];
	delete[] pNodes;
}

void cs350::bvh::find_nodes_to_merge(cs350::BVH_node_ptr* node, const int num_objs, int& first_obj, int& second_obj)
{
	float min_surface_area = FLT_MAX;

	for (int i = 0; i < num_objs; i++)
	{
		for (int j = i + 1; j < num_objs; j++)
		{
			float surfdace_area = compute_surface_area(node[i]->mBV, node[j]->mBV);

			if (surfdace_area < min_surface_area)
			{
				min_surface_area = surfdace_area;
				first_obj = i;
				second_obj = j;
			}
		}
	}
}

void cs350::bvh::insertion_tree(cs350::BVH_node** tree, std::vector<glm::vec3>* objects, int num_obj)
{
	cs350::BVH_node* new_node = new cs350::BVH_node();
	*tree = new_node;

	//create the first object that will be on the root
	new_node->mObject.push_back(0);
	new_node->mType = cs350::node_type::LEAF;
	new_node->mBV = compute_bounding_volume(&objects[0], 1);

	//go trought all the triangles to insert every one 
	for (int i = 1; i < num_obj; i++)
	{
		//create the new leaf that we gonna insert
		new_node = new cs350::BVH_node();
		new_node->mObject.push_back(i);
		new_node->mType = cs350::node_type::LEAF;
		new_node->mBV = compute_bounding_volume(&objects[i], 1);

		//insert the leaf into the root
		insert_node(tree, new_node);
	}
}

void cs350::bvh::insert_node(cs350::BVH_node** node, cs350::BVH_node* node_to_insert)
{
	//check if we are in a leaf
	if ((*node)->mType == node_type::LEAF)
	{
		//create a new node that will encapsule bouth bvh
		cs350::BVH_node* new_node = new cs350::BVH_node();
		new_node->mLeftNode = (*node);
		new_node->mRightNode = node_to_insert;
		new_node->mType = node_type::NODE;
		new_node->mBV = compute_bounding_volume((*node)->mBV, node_to_insert->mBV);
		//copy all the triangles
		new_node->mObject.insert(new_node->mObject.begin(), (*node)->mObject.begin()       , (*node)->mObject.end());
		new_node->mObject.insert(new_node->mObject.end()  , node_to_insert->mObject.begin(), node_to_insert->mObject.end());

		//finally we change the pointer to the new one
		*node = new_node;

	}
	else//its a node we need to check which way we will insert
	{
		//chose the best way and update the bv
		float option1 = compute_surface_area((*node)->mLeftNode->mBV , node_to_insert->mBV);
		float option2 = compute_surface_area((*node)->mRightNode->mBV, node_to_insert->mBV);
		//ERRORRR IS HEREEEEEEEE
		cs350::aabb best_option;
		if (option1 < option2)
		{
			best_option = compute_bounding_volume((*node)->mLeftNode->mBV, node_to_insert->mBV);
			(*node)->mBV = compute_bounding_volume(best_option, (*node)->mRightNode->mBV);
			(*node)->mObject.insert((*node)->mObject.end(), node_to_insert->mObject.begin(), node_to_insert->mObject.end());;
			insert_node(&(*node)->mLeftNode, node_to_insert);
		}
		else
		{
			best_option = compute_bounding_volume((*node)->mRightNode->mBV, node_to_insert->mBV);
			(*node)->mBV = compute_bounding_volume((*node)->mLeftNode->mBV, best_option);
			(*node)->mObject.insert((*node)->mObject.end(), node_to_insert->mObject.begin(), node_to_insert->mObject.end());;
			insert_node(&(*node)->mRightNode, node_to_insert);
		}
	}
}

float cs350::bvh::compute_surface_area(const cs350::aabb& object1, const cs350::aabb& object2)
{
	cs350::aabb aabb = compute_bounding_volume(object1, object2);

	float x_axis = aabb.mMax.x - aabb.mMin.x;
	float y_axis = aabb.mMax.y - aabb.mMin.y;
	float z_axis = aabb.mMax.z - aabb.mMin.z;

	float surface_area = (x_axis * y_axis) * 2 + (y_axis * z_axis) * 2 + (x_axis * z_axis) * 2;

	return surface_area;
}

cs350::aabb cs350::bvh::compute_bounding_volume(const cs350::aabb& object1, const cs350::aabb& object2)
{
	glm::vec3 max = object2.mMax;
	glm::vec3 min = object2.mMin;

	//get the min values
	if (object1.mMin.x < min.x)
		min.x = object1.mMin.x;
	if (object1.mMin.y < min.y)
		min.y = object1.mMin.y;
	if (object1.mMin.z < min.z)
		min.z = object1.mMin.z;

	//get thge max values
	if (object1.mMax.x > max.x)
		max.x = object1.mMax.x;
	if (object1.mMax.y > max.y)
		max.y = object1.mMax.y;
	if (object1.mMax.z > max.z)
		max.z = object1.mMax.z;

	return cs350::aabb{ min, max };
}

void cs350::bvh::delete_BVH(cs350::BVH_node* node)
{
	if (node == nullptr)
		return;

	delete_BVH(node->mLeftNode);
	delete_BVH(node->mRightNode);

	delete node;
	node = nullptr;
}

//TOP DOWN BVH

//14k of recursion before stack overflow
void cs350::bvh::top_down_bvt_tree(cs350::BVH_TD_node** tree, std::vector<glm::vec3>* objects, int num_obj)
{
	cs350::BVH_TD_node* new_node = new cs350::BVH_TD_node();
	*tree = new_node;
	//compute bounding volume for objects
	new_node->mBV = compute_bounding_volume(&objects[0], num_obj);

	//MIN OBJECT PER LEAF
	if (num_obj <= mMinimum_leaf_obj)
	{
		new_node->mType = cs350::node_type::LEAF;
		new_node->mNumbrerObj = num_obj;
		new_node->mObject = &objects[0]; //pointer to first object in leaf
	}
	else
	{
		new_node->mType = cs350::node_type::NODE;
		new_node->mNumbrerObj = num_obj;
		new_node->mObject = &objects[0]; //pointer to first object in leaf
		//based on the 
		int k = partition_objects(&objects[0], num_obj, new_node->mBV);
		//santy check in case we cant partition fromn the largest axes
		if (k == 0)
			return;

		// Recursively construct left and right subtree from subarrays and
		// point the left and right fields of the current node at the subtrees
		top_down_bvt_tree(&(new_node->mLeftNode), &objects[0], k);
		top_down_bvt_tree(&(new_node->mRightNode), &objects[k], num_obj - k);
	}

}

int cs350::bvh::partition_objects(std::vector<glm::vec3>* objects, int num_obj, const cs350::aabb& aabb)
{
	float distance_x = aabb.mMax.x - aabb.mMin.x;
	float distance_y = aabb.mMax.y - aabb.mMin.y;
	float distance_z = aabb.mMax.z - aabb.mMin.z;

	int axis = 0;
	int result = 0;

	if (distance_y >= distance_x && distance_y >= distance_z) axis = 1;
	if (distance_z >= distance_x && distance_z >= distance_y) axis = 2;

	float axis_center = 0.0f;

	for (int i = 0; i < num_obj; i++)
	{
		axis_center += (objects[i][0][axis] + objects[i][1][axis] + objects[i][2][axis]) / 3;
	}
	axis_center /= num_obj;

	for (int i = 0; i < num_obj; i++)
	{
		float center = 0;

		center += objects[i][0][axis] + objects[i][1][axis] + objects[i][2][axis];
		center /= 3;

		if (center < axis_center)
			result++;
	}

	// sort using a lambda expression 
	std::sort(&objects[0], &objects[num_obj],
		[&axis](std::vector<glm::vec3>& a, std::vector<glm::vec3> b)
		{
			return (a[0][axis] + a[1][axis] + a[2][axis]) / 3 > (b[0][axis] + b[1][axis] + b[2][axis]) / 3;
		});

	return result;
}

cs350::aabb cs350::bvh::compute_bounding_volume(std::vector<glm::vec3>* objects, int num_obj)
{
	glm::vec3 max(-FLT_MAX);
	glm::vec3 min(FLT_MAX);

	for (int i = 0; i < num_obj; i++)
	{
		for (int k = 0; k < 3; k++)
		{
			//get the min values
			if (objects[i][k].x < min.x)
				min.x = objects[i][k].x;
			if (objects[i][k].y < min.y)
				min.y = objects[i][k].y;
			if (objects[i][k].z < min.z)
				min.z = objects[i][k].z;

			//get thge max values
			if (objects[i][k].x > max.x)
				max.x = objects[i][k].x;
			if (objects[i][k].y > max.y)
				max.y = objects[i][k].y;
			if (objects[i][k].z > max.z)
				max.z = objects[i][k].z;
		}
	}

	return cs350::aabb{ min, max };
}

void cs350::bvh::delete_BVH(cs350::BVH_TD_node* node)
{
	if (node == nullptr)
		return;

	delete_BVH(node->mLeftNode);
	delete_BVH(node->mRightNode);

	delete node;
	node = nullptr;
}

cs350::BVH_TD_node* cs350::bvh::get_td_root()
{
	return mTDRoot;
}

void cs350::bvh::create_top_down_BVH(std::vector<glm::vec3>* objects, int size)
{
	if (mTDRoot != nullptr)
		delete_BVH(mTDRoot);

	mMinimum_leaf_obj = (int)((float)size * 0.10f);
	if (mMinimum_leaf_obj < 6) mMinimum_leaf_obj = 6;

	top_down_bvt_tree(&mTDRoot, &objects[0], size);
}

void cs350::bvh::create_bottom_up_BVH(std::vector<glm::vec3>* objects, int size)
{
	if (mBURoot != nullptr)
		delete_BVH(mBURoot);

	bottom_up_bvt_tree(&mBURoot, &objects[0], size);
}

void cs350::bvh::create_insertion_BVH(std::vector<glm::vec3>* objects, int size)
{
	if (mINSRoot != nullptr)
		delete_BVH(mINSRoot);

	insertion_tree(&mINSRoot, &objects[0], size);
}

void cs350::bvh::clear()
{
	delete_BVH(mTDRoot);
	delete_BVH(mBURoot);
	delete_BVH(mINSRoot);

	mBURoot  = nullptr;
	mTDRoot  = nullptr;
	mINSRoot = nullptr;
}
