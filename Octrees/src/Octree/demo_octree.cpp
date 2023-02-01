#include "Helpers/pch.hpp"
#include "Render/mesh.hpp"
#include "Shader/shader.hpp"
#include "Helpers/opengl.hpp"
#include "Render/renderer.hpp"
#include "Helpers/imgui.hpp"
#include "demo_octree.hpp"

namespace cs350 {

    namespace {

        /**
         * @brief
		 *  Renders a mesh using the plain color shader. (specific to this demo)		 
		 * @param mesh
		 * @param m2w
		 * @param color
		 */
        void debug_draw_phong(std::shared_ptr<mesh> const& mesh, const glm::mat4& m2w, glm::vec4 const& color)
        {
            // Objects
            auto& inst   = renderer::instance();
            auto& shd    = inst.resources().mShaders.phong;
            auto& camera = inst.camera();

            // Parameters
            auto const& v              = camera.get_view_matrix();
            auto const& p              = camera.get_perspective_matrix();
            auto        mv             = v * m2w;
            auto        mvp            = p * mv;
            auto        normal_matrix  = glm::mat3(glm::transpose(glm::inverse(mv)));
            auto        view_pos       = glm::vec3(500, 500, 100);
            auto        view_light_pos = v * glm::vec4(view_pos, 1);

            // OpenGL set
            shd.use();
            shd.set_uniform("uniform_mvp", mvp);                                  // Note: expensive
            shd.set_uniform("uniform_color", color);                              // Note: expensive
            shd.set_uniform("uniform_mv", mv);                                    // Note: expensive
            shd.set_uniform("uniform_normal_matrix", normal_matrix);              // Note: expensive
            shd.set_uniform("uniform_view_light_pos", glm::vec3(view_light_pos)); // Note: expensive

            // Draw
            mesh->bind();
            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(mesh->mMeshData.positions.size()));
        }

        /**
		 * @param window
		 * @param key
		 * @param scancode
		 * @param action
		 * @param mods
		 */
        void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            // ImGui has priority
            if (ImGui::GetIO().WantCaptureKeyboard)
            {
                return;
            }

            auto demo = reinterpret_cast<demo_octree*>(glfwGetWindowUserPointer(renderer::instance().window().handle()));
            if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
            {
                demo->options().physics_enabled ^= 1u;
            }
            if (key == 'R' && action == GLFW_PRESS)
            {
                demo->destroy();
            }
        }

        /**
		 *
		 * @param window
		 * @param button
		 * @param action
		 * @param mods
		 */
        void mouse_callback(GLFWwindow* window, int button, int action, int mods)
        {
            // ImGui has priority
            if (ImGui::GetIO().WantCaptureMouse)
            {
                return;
            }

            auto demo = reinterpret_cast<demo_octree*>(glfwGetWindowUserPointer(renderer::instance().window().handle()));

            // Shoot on click
            if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
                demo->shoot(glm::linearRand(1.0f, 5.0f));
            }
            if (button == GLFW_MOUSE_BUTTON_3 && action == GLFW_PRESS) {
                demo->shoot(0.0f);
            }
        }
    }

    void demo_octree::check_collisions_octree(octree<physics_object>::node* node)
    {
        //we finished
        if (node == nullptr)
            return;

        std::vector<octree<physics_object>::node*> childs;
        //NEED TO GET THE CHILD OF THE CHILDS
        get_all_childs(node, childs);

        //if we have objects to check
        if (node->mFirst != nullptr)
        {
            physics_object* obj1 = node->mFirst;
            physics_object* obj2 = node->mFirst->octree_next_object;

            while (obj1)
            {
                //we check with the actual node
                while (obj2)
                {
                    check_intersection(obj1, obj2);
                    obj2 = obj2->octree_next_object;
                }

                for (auto cld : childs)
                {
                    physics_object* obj3 = cld->mFirst;

                    while (obj3)
                    {
                        check_intersection(obj1, obj3);
                        obj3 = obj3->octree_next_object;
                    }
                }

                obj1 = obj1->octree_next_object;
                if (obj1 != nullptr)
                    obj2 = obj1->octree_next_object;
            }
        }
        //we call the childs
        for (auto* child : childs)
        {
            check_collisions_octree(child);
        }
    }

    void demo_octree::get_all_childs(octree<physics_object>::node* node, std::vector<octree<physics_object>::node*>& childs)
    {
        //we get the childs
        for (int i = 0; i < 8; i++)
        {
            if (((node->mChildrenActive >> i) & 1) == 1)
            {
                uint32_t new_locational_code = node->mLocationalCode;
                new_locational_code <<= 3;
                new_locational_code |= i;

                octree<physics_object>::node* child = m_octree_dynamic.find_node(new_locational_code);
                if (child)
                {
                    get_all_childs(child, childs);
                    childs.push_back(child);
                }
            }
        }
    }

    /**
	 * @brief
	 */
    demo_octree::~demo_octree()
    {
        cs350::renderer::instance().destroy();
        destroy();
    }

    /**
	 *
	 */
    void demo_octree::create()
    {
        { // Window/GFX
            renderer::instance().create(1280, 720, "CS350", false);

            // ImGui
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGui::StyleColorsDark();
            ImGui_ImplGlfw_InitForOpenGL(renderer::instance().window().handle(), true);
            ImGui_ImplOpenGL3_Init("#version 430");

            // Input
            glfwSetWindowUserPointer(renderer::instance().window().handle(), this);
            glfwSetKeyCallback(renderer::instance().window().handle(), key_callback);
            glfwSetMouseButtonCallback(renderer::instance().window().handle(), mouse_callback);
        }

        // Octree
        //
        m_octree_dynamic.set_root_size(1u << m_options.octree_size_bit);
        m_octree_dynamic.set_levels(m_options.octree_levels);

    }

    /**
	 *
	 * @return true Everything is ok
	 * @return false Window closed
	 */
    bool demo_octree::update()
    {
        auto& window = renderer::instance().window();
        if (!window.keep_updating())
        {
            return false;
        }
        //window.update();

        // Frame start
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glDepthMask(GL_TRUE);
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, (GLsizei)window.size().x, (GLsizei)window.size().y);

        float dt = (float)ImGui::GetIO().DeltaTime;

        // Camera update
        update_camera(dt);

        // Physics update
        for (auto* obj : m_dynamic_objects)
        {
            if (m_options.physics_enabled)
            {
                // Phy
                obj->position = obj->position + obj->velocity * dt;
            }

            //CHANGE THE DIRECTION OF THE BALLS DEPENDING ON THE OCTREE
            // Clamping and bouncing on walls
            float boundary = m_octree_dynamic.root_size() * 0.5f;
            boundary -= 5.0f; // Make them bounce before boundary
            for (int i = 0; i < 3; ++i)
            {
                if (obj->position[i] > boundary && obj->velocity[i] > 0)
                {
                    obj->position[i] = boundary;
                    obj->velocity[i] *= -1;
                }
                if (obj->position[i] < -boundary && obj->velocity[i] < 0)
                {
                    obj->position[i] = -boundary;
                    obj->velocity[i] *= -1;
                }
            }

            // BV (Compute BV)
            obj->bv_world = aabb(obj->position , obj->radius);

             //
            {   // Octree update
                //if its a new octree_node
                if (obj->octree_node == nullptr)
                {
                    //create or find the node in which the object will be
                    obj->octree_node = m_octree_dynamic.find_create_node(obj->bv_world);
                    //put the new object at the beginning of the list O(1)
                    if(obj->octree_node->mFirst != nullptr)//check if we need to update any object
                    {
                        obj->octree_next_object = obj->octree_node->mFirst;
                        obj->octree_node->mFirst->octree_prev_object = obj;
                    }
                    obj->octree_node->mFirst = obj;
                }
                else//we check if the object is not in the same node anymore
                {
                    octree<physics_object>::node* actual_node = m_octree_dynamic.find_node(obj->bv_world);

                    //need to chage or create a new node
                    if (actual_node == nullptr
                        || actual_node->mLocationalCode != obj->octree_node->mLocationalCode)
                    {
                        //update the node and check if we need to delete it
                        //check if we have a previous
                        if (obj->octree_prev_object != nullptr)
                        {
                            obj->octree_prev_object->octree_next_object = obj->octree_next_object;
                        }
                        else//we are mFirst object
                        {
                            obj->octree_node->mFirst = obj->octree_next_object;
                        }
                        //check if we have a next
                        if (obj->octree_next_object != nullptr)
                        {
                            obj->octree_next_object->octree_prev_object = obj->octree_prev_object;
                        }

                        //check if we need to delete
                        if (obj->octree_node->mFirst == nullptr && obj->octree_node->mChildrenActive == 0)
                        {
                            m_octree_dynamic.delete_node(obj->octree_node->mLocationalCode);
                        }

                        //create or find the node in which the object will be
                        obj->octree_node = m_octree_dynamic.find_create_node(obj->bv_world);
                        obj->octree_next_object = nullptr;
                        obj->octree_prev_object = nullptr;

                        //put the new object at the beginning of the list O(1)
                        if (obj->octree_node->mFirst != nullptr)//check if we need to update any object
                        {
                            obj->octree_next_object = obj->octree_node->mFirst;
                            obj->octree_node->mFirst->octree_prev_object = obj;
                        }
                        obj->octree_node->mFirst = obj;
                    }
                }
            }
        }

        // Render
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        glDepthFunc(GL_LESS);
        glDepthMask(GL_TRUE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        //Render all the objects and highlight them if we need to
        for (auto* obj : m_dynamic_objects) 
        {
            auto  m2w   = glm::translate(obj->position) * glm::scale(glm::vec3(obj->radius));
            auto& mesh  = renderer::instance().resources().mMesh.sphere;
            auto  color = glm::vec4(0.5, 0.5, 0.5, 1);


            // Highlight object?
            if (m_options.highlight_level != -1 &&
                obj->octree_node &&
                locational_code_depth(obj->octree_node->mLocationalCode) == m_options.highlight_level)
            {
                color = {0, 1, 0, 1};
            }

            cs350::debug_draw_aabb(obj->bv_world, color, true);
            debug_draw_phong(mesh, m2w, color);
        }

        if (m_options.debug_octree)
        {
            // Debug draw nodes of the selected highlight-level
            //all levels if -1
            m_octree_dynamic.debug_draw_nodes(m_options.highlight_level);

        }


        { // All pairs debug, BRUTE FORCE
            if (m_options.brute_force) 
            {
                for (size_t i = 0; i < m_dynamic_objects.size(); ++i) 
                {
                    for (size_t j = i + 1; j < m_dynamic_objects.size(); ++j)
                    {
                        check_intersection(m_dynamic_objects[i], m_dynamic_objects[j]);
                    }
                }
            } 
            else // Octree all pairs (top-down or bottom up)
            {
                //top down aproach
                //we get the root and we go to the childs 
                check_collisions_octree(m_octree_dynamic.find_node(0b1));
            }
        }

        // Help, IMGUI
        if (ImGui::Begin("Help"))
        {
            ImGui::Text("Help: \n"
                        "\tLook:      Right click + mouse\n"
                        "\tMove:      WASD\n"
                        "\tMove fast: SHIFT+WASD\n"
                        "\tPause:     SPACE\n"
                        "\tRecreate:  r");

            if (ImGui::SliderInt("Octree size", &m_options.octree_size_bit, 1, 32))
            {
                // If octree size changes, orphan everything (forces reinsertion)
                m_octree_dynamic.destroy();
                m_octree_dynamic.set_root_size(1u << m_options.octree_size_bit);
                for (auto obj : m_dynamic_objects) {
                    obj->octree_node        = nullptr;
                    obj->octree_next_object = nullptr;
                    obj->octree_prev_object = nullptr;
                }
            }

            if (ImGui::SliderInt("Octree levels", &m_options.octree_levels, 1, 10))
            {
                // If octree max levels changes, orphan everything (forces reinsertion)
                m_octree_dynamic.destroy();
                m_octree_dynamic.set_levels(m_options.octree_levels);
                for (auto obj : m_dynamic_objects) {
                    obj->octree_node        = nullptr;
                    obj->octree_next_object = nullptr;
                    obj->octree_prev_object = nullptr;
                }
            }

            ImGui::SliderInt("Highlight level", &m_options.highlight_level, -1, m_options.octree_levels);

            ImGui::Checkbox("Octree debug render", &m_options.debug_octree);
            ImGui::Checkbox("Pair debug render", &m_options.debug_intersections);
            ImGui::Checkbox("Physics enabled", &m_options.physics_enabled);
            ImGui::Checkbox("Brute force", &m_options.brute_force);
            if (ImGui::Button("Random"))
            {
                for (int i = 0; i < 10; ++i) 
                {
                    float boundary = (float)m_octree_dynamic.root_size();
                    boundary -= 5.0f; // Make them not bounce outside
                    auto  p = glm::linearRand(glm::vec3(boundary) * -0.5f, glm::vec3(boundary) * 0.5f);
                    auto  v = glm::ballRand(glm::linearRand(1.0f, 5.0f));
                    float r = glm::linearRand(0.5f, 2.0f);

                    // Create
                    auto obj      = new physics_object;
                    obj->position = p;
                    obj->velocity = v;
                    obj->radius   = r;
                    m_dynamic_objects.push_back(obj);
                }
            }

            // Keep track of checks
            m_options.checks_history.push_back((float)m_options.checks_this_frame);
            if (m_options.checks_history.size() > 500) 
            {
                m_options.checks_history.erase(m_options.checks_history.begin());
            }
            m_options.checks_this_frame = 0;

            ImGui::Text("Objects: %d", int(m_dynamic_objects.size()));
            ImGui::Text("Intersection checks: %d", int(m_options.checks_history.back()));
            ImGui::PlotLines("", m_options.checks_history.data(), (int)m_options.checks_history.size(), 0, "", 0, FLT_MAX, ImVec2(0, 64));
            ImGui::Text("Max: %d", static_cast<int>(*std::max_element(m_options.checks_history.begin(), m_options.checks_history.end())));
        }
        ImGui::End();

        // Frame end
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffers
        glfwSwapBuffers(window.handle());
        glfwPollEvents();
        return true;
    }

    /**
	 * @brief
	 */
    void demo_octree::destroy()
    {
        //delete all the objects
        for (auto* it : m_dynamic_objects)
        {
            delete it;
        }
        m_dynamic_objects.clear();

        m_octree_dynamic.destroy();
    }

    /**
	 * @brief
	 */
    void demo_octree::shoot(float v)
    {
        auto const& camera          = renderer::instance().camera();
        auto        camera_dir      = camera.get_target() - camera.get_position();
        auto        camera_position = camera.get_position();

        auto obj = new physics_object;

        obj->position = camera_position;
        obj->velocity = camera_dir * v;
        obj->radius   = glm::linearRand(0.5f, 2.0f);

        m_dynamic_objects.push_back(obj);
    }

    /**
	 * @brief
     *  Placeholder intersection test, only draws and keeps track of statistics
	 * @param a
	 * @param b
	 */
    void demo_octree::check_intersection(physics_object const* a, physics_object const* b)
    {
        if (m_options.debug_intersections) {
            if (m_options.highlight_level == -1 ||
                (b->octree_node && locational_code_depth(b->octree_node->mLocationalCode) == m_options.highlight_level) ||
                (a->octree_node && locational_code_depth(a->octree_node->mLocationalCode) == m_options.highlight_level)) {
                glDisable(GL_CULL_FACE);
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_LESS);
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glDepthMask(GL_FALSE);
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                debug_draw_segment({a->position + glm::vec3{0, a->radius, 0}, b->position}, {1, 0, 1, 0.25f});
            }
        }
        m_options.checks_this_frame++;
    }

    /**
     * @brief
     *  Provided camera controls
     * @param dt 
     */
    void demo_octree::update_camera(float dt)
    {
        auto& camera          = renderer::instance().camera();
        auto& window          = renderer::instance().window();
        auto* glfwWindow      = window.handle();
        auto  camera_dir      = camera.get_target() - camera.get_position();
        auto  camera_position = camera.get_position();

        // Cursor
        double cursor_x = 0.0;
        double cursor_y = 0.0;
        glfwGetCursorPos(glfwWindow, &cursor_x, &cursor_y);
        { // Input
            if (glfwGetMouseButton(glfwWindow, GLFW_MOUSE_BUTTON_2))
            {
                float speed = 20.0f;
                auto  side  = glm::normalize(glm::cross(camera_dir, {0, 1, 0}));

                if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT_SHIFT))
                {
                    speed *= 5.0f;
                }
                // Move
                if (glfwGetKey(glfwWindow, GLFW_KEY_W))
                {
                    camera_position += glm::normalize(camera_dir) * dt * speed;
                }
                if (glfwGetKey(glfwWindow, GLFW_KEY_S))
                {
                    camera_position -= glm::normalize(camera_dir) * dt * speed;
                }
                if (glfwGetKey(glfwWindow, GLFW_KEY_A)) 
                {
                    camera_position -= glm::normalize(side) * dt * speed;
                }
                if (glfwGetKey(glfwWindow, GLFW_KEY_D))
                {
                    camera_position += glm::normalize(side) * dt * speed;
                }

                // View
                glm::vec2 cursor_delta = {(float)cursor_x - m_cursor_pos.x, (float)cursor_y - m_cursor_pos.y};
                camera_dir             = glm::vec3(glm::vec4(camera_dir, 0) * glm::rotate(glm::radians(15.0f) * 0.01f * cursor_delta.y, side));
                camera_dir             = glm::vec3(glm::vec4(camera_dir, 0) * glm::rotate(glm::radians(15.0f) * 0.01f * cursor_delta.x, glm::vec3(0, 1, 0)));
            }
            m_cursor_pos       = {(float)cursor_x, (float)cursor_y};
            auto camera_target = camera_position + camera_dir;

            // Submit changes
            camera.set_position(camera_position);
            camera.set_target(camera_target);
            camera.set_projection(60.0f, window.size(), 0.01f, 10000.0f);
            camera.update();
        }
    }
}
