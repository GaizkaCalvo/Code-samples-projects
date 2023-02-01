#pragma once

#include "Camera/camera.hpp"
#include "Window/window.hpp"
#include "octree.hpp"

namespace cs350 {
    /**
     * @brief
     *  Describes a basics physical object
     */
    struct physics_object
    {
        // Object data
        glm::vec3 position = { 0.0f, 0.0f , 0.0f };
        float     radius = 0.0f;
        glm::vec3 velocity = { 0.0f, 0.0f , 0.0f };
        aabb      bv_world = {};

        // Space partitioning data
        //node in which we are
        octree<physics_object>::node* octree_node{};
        //next or previous object in the node(level) that we are in
        physics_object*               octree_next_object{};
        physics_object*               octree_prev_object{};
    };

    /**
     * @brief
     *  Specific demo for this assignment
     */
    class demo_octree
    {
      private:
        // Others
        glm::vec2 m_cursor_pos = {};

        //
        octree<physics_object>       m_octree_dynamic;
        std::vector<physics_object*> m_dynamic_objects;

        void check_collisions_octree(const octree<physics_object>::node* node);
        void get_all_childs(const octree<physics_object>::node* node, std::vector<const octree<physics_object>::node*>& childs);

        // Imgui options
        struct
        {
            bool debug_octree{true};
            bool debug_intersections{true};
            bool physics_enabled{true};
            int  octree_size_bit{7};
            int  octree_levels{3};
            bool brute_force{false};
            int  highlight_level{-1};

            // Performance counters
            int                checks_this_frame{};
            std::vector<float> checks_history;
        } m_options;

      public:
        ~demo_octree();
        void create();
        bool update();
        void destroy();
        void shoot(float v);
        void check_intersection(physics_object const* a, physics_object const* b);
        void update_camera(float dt);

        decltype(m_options)& options() { return m_options; }
    };
}
