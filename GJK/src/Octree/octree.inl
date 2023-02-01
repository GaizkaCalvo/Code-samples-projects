#include "octree.hpp"
#include "Debug/debug.hpp"



namespace cs350 {
    class physics_object;
    /**
    * @brief: we compute the locational code with the given position, root size and root level 
    * @param  world_position: positions that we convert to locational code
    * @param  root_size: size of the root
    * @param  levels: levels of the root
    * @return: we return the locational code that we create
    */
    template<int dimension>
    uint32_t compute_locational_code(glm::vec<dimension, int> world_position, uint32_t root_size, uint32_t levels)
    {
        //Some sanity checks and offset the values 
        if ((uint32_t)glm::abs(world_position[0]) > root_size/2)
            return 1;
        uint32_t x_pos = (uint32_t)(world_position[0] + root_size / 2);
        if ((uint32_t)glm::abs(world_position[1]) > root_size/2)
            return 1;
        uint32_t y_pos = (uint32_t)(world_position[1] + root_size / 2);
        uint32_t z_pos;
        if (dimension == 3)
        {
            if ((uint32_t)glm::abs(world_position[2]) > root_size/2)
                return 1;
            z_pos = (uint32_t)(world_position[2] + root_size / 2);
            if(x_pos == root_size || y_pos == root_size || z_pos == root_size)
                return 1;
        }
        else
        {
            if (x_pos == root_size || y_pos == root_size)
                return 1;
        }
        
        //we see where the 1 is located in binary
        int bit_position = glm::log2(root_size) - 1;

        //insert the root
        uint32_t result = 0;
        result = result | (1 << dimension * levels);
        //another sanity check
        
        if (x_pos == 0 && y_pos == 0)
        {
            return result;
        }
        if(dimension == 3)
            if (z_pos == 0 && x_pos == 0 && y_pos == 0)
                return result;


        //then we calculate the locational code that we need 
        for (int i = levels; i > 0; i--)
        {
            if(((x_pos >> bit_position) & 1) == 1)
                result = result | (1 << (dimension * i - dimension));
            if (((y_pos >> bit_position) & 1) == 1)
                result = result | (1 << (dimension * i - (dimension - 1)));
            if (dimension == 3)
                if (((z_pos >> bit_position) & 1) == 1)
                    result = result | (1 << (dimension * i - (dimension - 2)));

            bit_position--;
        }
        //return the result
        return result;
    }
    /**
    * @brief: we update the parent nodes of the given locational code 
    * @param  locational_code: locational code of the node we are in
    * @param  active: activate or deactivate the bit in the uint8 the node store
    */
    template<typename T>
    inline void octree<T>::update_parent(uint32_t locational_code, bool active)
    {
        //we are in the root
        if (locational_code == 1)
        {
            return;
        }

        //we get the location code of the child
        uint32_t new_locational_code = locational_code >> 3;
        uint32_t child = locational_code & 0b111;
        octree<physics_object>::node* new_node = find_create_node(new_locational_code);

        //activate o deactivate child
        if (active)
            new_node->mChildrenActive |= (1 << child);
        else
            new_node->mChildrenActive &= ~(1 << child);

        if (new_node->mChildrenActive == 0 && new_node->mFirst == nullptr)
        {
            delete mNodes.at(new_locational_code);
            mNodes.erase(new_locational_code);
            update_parent(new_locational_code, active);
        }
    }
    /**
   * @brief: constructor
   */
    template<typename T>
    inline octree<T>::octree() : mRootSize(0), mLevels(0)
    {
    }
    /**
   * @brief: destructor
   */
    template<typename T>
    inline octree<T>::~octree()
    {
        destroy();
    }
    /**
   * @brief: we clear all the nodes that we have
   */
    template<typename T>
    inline void octree<T>::destroy()
    {
        for (auto& it : mNodes)
        {
            delete it.second;
        }
        mNodes.clear();
    }
    /**
   * @brief: we draw the nodes that are created, -1 to draw all 
   * @param  level: level in which the nodes that we need to draw are
   */
    template<typename T>
    inline void octree<T>::debug_draw_nodes(uint32_t level)
    {
        for (auto& it : mNodes)
        {
            if (level == -1 || locational_code_depth(it.second->mLocationalCode) == level)
            {
                cs350::aabb debug_draw = compute_bv(it.second->mLocationalCode, mRootSize);
                cs350::debug_draw_aabb(debug_draw, { 0.0f, 1.0f, 1.0f, 1.0f }, true);
            }
        }
    }
    /**
   * @brief: we find or we create a node if we dont find one and return it
   * @param  bv: bounding volume we pass to create the node
   * @return: node that we found or created
   */
    template<typename T>
    inline typename octree<T>::node* octree<T>::find_create_node(aabb const& bv)
    {
        uint32_t locational_code =  compute_locational_code(bv, mRootSize, mLevels);

        return find_create_node(locational_code);
    }
    /**
   * @brief: we find the node for the given bv, nulll if we didnt find one
   * @param  bv: bounding volume that we use to compute the locational code
   * @return: node that we found
   */
    template<typename T>
    inline typename octree<T>::node* octree<T>::find_node(aabb const& bv)
    {
        uint32_t locational_code = compute_locational_code(bv, mRootSize, mLevels);

        if (mNodes.find(locational_code) == mNodes.end())
            return nullptr;

        return mNodes.at(locational_code);
    }
    /**
   * @brief:  we find the node for the given bv, nulll if we didnt find one
   * @param  bv: bounding volume that we use to compute the locational code
   * @return: node that we found
   */
    template<typename T>
    inline typename octree<T>::node const* octree<T>::find_node(aabb const& bv) const
    {
        return find_node(bv);
    }
    /**
    * @brief: we find the node for the given bv, nulll if we didnt find one
    * @param  locational_code: locational code volume that we use to find the node
    * @return: node that we found
    */ 
    template<typename T>
    inline typename octree<T>::node* octree<T>::find_create_node(uint32_t locational_code)
    {
        //if the node doesnt exist
        if (mNodes.find(locational_code) == mNodes.end())
        {
            //we create the new node and we insert it to the table
            octree<T>::node* new_node = new octree<T>::node;
            new_node->mLocationalCode = locational_code;
            new_node->mChildrenActive = 0;
            new_node->mFirst = nullptr;

            mNodes.emplace(locational_code, new_node);

            update_parent(locational_code, true);
        }

        //we return the value that we find
        return mNodes.at(locational_code);
    }
    /**
   * @brief:  we find the node for the given bv, nulll if we didnt find one
   * @param  locational_code: locational code that we use to find the node
   * @return: node that we found
   */
    template<typename T>
    inline typename octree<T>::node* octree<T>::find_node(uint32_t locational_code)
    {
        if (mNodes.find(locational_code) != mNodes.end())
            return mNodes.at(locational_code);

        return nullptr;
    }
    /**
    * @brief:  we find the node for the given bv, nulll if we didnt find one
    * @param  locational_code: locational code that we use to find the node
    * @return: node that we found
    */
    template<typename T>
    inline typename octree<T>::node const* octree<T>::find_node(uint32_t locational_code) const
    {
        return find_node(locational_code);
    }
    /**
   * @brief: we delete the node that they pass and update the parents
   * @param  locational_code: to find the node
   */
    template<typename T>
    inline typename void octree<T>::delete_node(uint32_t locational_code)
    {
        update_parent(locational_code, false);
        delete mNodes.at(locational_code);
        mNodes.erase(locational_code);
    }
}