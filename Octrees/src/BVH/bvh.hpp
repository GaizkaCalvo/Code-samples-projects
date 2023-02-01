/**
 * @file nodes.hpp
 * @author Gaizka Calvo (gaizka.c@digipen.edu)
 * @date 21/09/2020
 *
 * @copyright Copyright (c) 2020
 * @brief Class we will use to manage the bv
 */
#pragma once
#include "BVH/nodes.hpp"

namespace cs350
{

	class bvh
	{
	public:
		 bvh();
		~bvh();

		cs350::BVH_TD_node* get_td_root();
		cs350::BVH_node*	get_bu_root();
		cs350::BVH_node*	get_ins_root();

		void create_top_down_BVH (std::vector<glm::vec3>* objects, int size);
		void create_bottom_up_BVH(std::vector<glm::vec3>* objects, int size);
		void create_insertion_BVH(std::vector<glm::vec3>* objects, int size);

		void clear();

		//bvh
		bool mDrawBVHTriangles;
		bool mDrawBVHTrianglesFaces;

	private:

		int mMinimum_leaf_obj;

		cs350::BVH_TD_node* mTDRoot;
		cs350::BVH_node*	mBURoot;
		cs350::BVH_node*	mINSRoot;

		//TOP DOWN
		void		top_down_bvt_tree		(cs350::BVH_TD_node** tree, std::vector<glm::vec3>* objects, int num_obj);
		int			partition_objects		(std::vector<glm::vec3>* objects, int num_obj, const cs350::aabb& aabb) const;
		void		delete_BVH				(cs350::BVH_TD_node* node);

		//BOTTOM UP
		void		 bottom_up_bvt_tree		(cs350::BVH_node** tree, std::vector<glm::vec3>* objects, int num_obj);
		void		 find_nodes_to_merge	(cs350::BVH_node_ptr* nodes, const int num_objs, int& i, int& j) const;
		
		//INSERTION
		void		insertion_tree	(cs350::BVH_node** tree, std::vector<glm::vec3>* objects, int num_obj);
		void		insert_node		(cs350::BVH_node** node, cs350::BVH_node* node_to_insert);

		//BOTTOM UP AND INSERTION
		float		 compute_surface_area	(const cs350::aabb& object1, const cs350::aabb& object2) const;
		float		 compute_surface_area	(const cs350::aabb& object1) const;
		cs350::aabb  compute_bounding_volume(const cs350::aabb& object1, const cs350::aabb& object2) const;
		void		 delete_BVH				(cs350::BVH_node* node);

		//TOP DONW, BOTTOM UP AND INSERTION 
		cs350::aabb compute_bounding_volume(std::vector<glm::vec3>* objects, int num_obj);
	};

}