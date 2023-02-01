/**
 * @file octree.hpp
 * @author Gaizka Calvo (gaizka.c@digipen.edu)
 * @date 21/09/2020
 *
 * @copyright Copyright (c) 2020
 * @brief Class that will encapsulate the management of the octree
 */
#ifndef CS350_OCTREE_TEST_OCTREE_HPP
#define CS350_OCTREE_TEST_OCTREE_HPP

namespace cs350 {

    template <int dimension = 3>
    uint32_t compute_locational_code(glm::vec<dimension, int> world_position, uint32_t root_size, uint32_t levels);
    uint32_t compute_locational_code(aabb const& bv, uint32_t root_size, uint32_t levels);
    aabb     compute_bv(uint32_t locational_code, uint32_t root_size);
    uint32_t locational_code_depth(uint32_t lc);
    uint32_t common_locational_code(uint32_t lc1, uint32_t lc2);

    /**
     * @brief
     * 	Linear octree, each node stores a head for a linked list of T
     * @tparam T
     */
    template <typename T>
    class octree
    {
      public:
        struct node
        {
            uint32_t mLocationalCode;//location code of the node
            uint8_t  mChildrenActive;//see how many childs has active 
                                     //0 0 0 0 0 0 0 0 --> 
            T*       mFirst;         //pointer to the first object
        };

      private:
        //hash table of linked list
        std::unordered_map<uint32_t, node*> mNodes;
        uint32_t                            mRootSize;
        uint32_t                            mLevels;

        void update_parent(uint32_t locational_code, bool active);

      public:
        octree();
        ~octree();
        void        destroy();

        void debug_draw_nodes(uint32_t level);

        //we create the new node if we need to while we find for the possible location 
        node*       find_create_node(aabb const& bv);
        node*       find_create_node(uint32_t locational_code);

        //we find the node where he at
        node*       find_node(aabb const& bv);
        node const* find_node(aabb const& bv) const;

        //we find the node where he at
        node*       find_node(uint32_t locational_code);
        node const* find_node(uint32_t locational_code) const;
        //we delete the selected node
        void        delete_node(uint32_t locational_code);

        [[nodiscard]] uint32_t root_size() const { return mRootSize; }
        void                   set_root_size(uint32_t size) { mRootSize = size; }
        void                   set_levels(uint32_t levels) { mLevels = levels; }
    };
}

#include "octree.inl" 

#endif //CS350_OCTREE_TEST_OCTREE_HPP
