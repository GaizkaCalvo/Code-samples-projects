#include "Helpers/pch.hpp"
#include "Editor/editor.hpp"
#include "Helpers/imgui.hpp"
#include "Render/renderer.hpp"
#include "GameObjects/object_manager.hpp"

cs350::editor::editor() : show_demo_window(false), show_another_window(false), clear_color((0.45f, 0.55f, 0.60f, 1.00f))
{
}

cs350::editor::~editor()
{
}

void cs350::editor::initialize()
{
    cs350::renderer & ren = cs350::renderer::instance();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(ren.window().get_window(), true);
    const char* glsl_version = "#version 430";
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void cs350::editor::update()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    bounding_volumes();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {
        ImGui::ShowDemoWindow(&show_demo_window);

        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }
    }


    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(cs350::renderer::instance().window().get_window());
    glfwPollEvents();
}

void cs350::editor::bounding_volumes()
{
    ObjectManager * obj_mng = ObjectManager::get_instance();
    GameObject * demo_object = obj_mng->FindGameObject("Blusti");

    if (demo_object != nullptr)
    {
        cs350::renderer& ren = cs350::renderer::instance();
        //start of the window
        ImGui::Begin("Model properties");

        //a little header to chose the model of the object
        if (ImGui::CollapsingHeader("Model"))
        {
            if (ImGui::Button("Sphere"))
            {
                demo_object->mTransform.mScale = glm::vec3(10.0f, 10.0f, 10.0f);
                demo_object->change_model(ren.resources().mMesh.sphere);
            }
            if (ImGui::Button("Cube"))
            {
                demo_object->mTransform.mScale = glm::vec3(10.0f, 10.0f, 10.0f);
                demo_object->change_model(ren.resources().mMesh.cube);
            }
            if (ImGui::Button("Bunny"))
            {
                demo_object->mTransform.mScale = glm::vec3(100.0f, 100.0f, 100.0f);
                demo_object->change_model(ren.resources().mMesh.bunny);
            }
            if (ImGui::Button("Gourd"))
            {
                demo_object->mTransform.mScale = glm::vec3(10.0f, 10.0f, 10.0f);
                demo_object->change_model(ren.resources().mMesh.gourd);
            }
            if (ImGui::Button("Sponza"))
            {
                demo_object->mTransform.mScale = glm::vec3(10.0f, 10.0f, 10.0f);
                demo_object->change_model(ren.resources().mMesh.sponza);
            }
            if (ImGui::Button("Icosahedron"))
            {
                demo_object->mTransform.mScale = glm::vec3(10.0f, 10.0f, 10.0f);
                demo_object->change_model(ren.resources().mMesh.icosahedron);
            }

            //model render options
            ImGui::Checkbox("Render faces", &demo_object->mFaceRender);

            ImGui::Checkbox("Debug lines", &demo_object->mDebugLines);
        }

        //a little header to chose the BVH type of the object
        if (ImGui::CollapsingHeader("BVH Types"))
        {
            if (ImGui::Button("Top down"))
                demo_object->mBVH.create_top_down_BVH(&demo_object->mMod->mMeshData.triangles[0], (int)demo_object->mMod->mMeshData.triangles.size());

            if (ImGui::Button("Bottom up"))
                demo_object->mBVH.create_bottom_up_BVH(&demo_object->mMod->mMeshData.triangles[0], (int)demo_object->mMod->mMeshData.triangles.size());

            if (ImGui::Button("Insertion"))
                demo_object->mBVH.create_insertion_BVH(&demo_object->mMod->mMeshData.triangles[0], (int)demo_object->mMod->mMeshData.triangles.size());

            //model render options
            ImGui::Checkbox("Draw triangles", &demo_object->mBVH.mDrawBVHTriangles);

            //SHOW TOP DOWN
            cs350::BVH_TD_node* td_root = demo_object->mBVH.get_td_root();
            if(td_root != nullptr)
                if (ImGui::TreeNode("Top Down BVH Root"))
                {
                    ImGui::Text("Debug draw ");
                    ImGui::SameLine();
                    //node render options
                    if (td_root->mShowMesh)
                    {
                        if (ImGui::SmallButton("Deactivate"))
                            td_root->mShowMesh = false;
                    }
                    else
                    {
                        if (ImGui::SmallButton("Activate"))
                            td_root->mShowMesh = true;
                    }
                    //ImGui::Checkbox("Debug lines", &root->mShowMesh);

                    show_bvh_nodes(td_root, "Root");
                    ImGui::TreePop();
                }

            //SHOW BOTTOM UP
            cs350::BVH_node* bu_root = demo_object->mBVH.get_bu_root();
            if (bu_root != nullptr)
                if (ImGui::TreeNode("Bottom Up BVH Root"))
                {
                    ImGui::Text("Debug draw ");
                    ImGui::SameLine();
                    //node render options
                    if (bu_root->mShowMesh)
                    {
                        if (ImGui::SmallButton("Deactivate"))
                            bu_root->mShowMesh = false;
                    }
                    else
                    {
                        if (ImGui::SmallButton("Activate"))
                            bu_root->mShowMesh = true;
                    }
                    //ImGui::Checkbox("Debug lines", &root->mShowMesh);

                    show_bvh_nodes(bu_root, "Root");
                    ImGui::TreePop();
                }

            //SHOW INSERTION
            cs350::BVH_node* ins_root = demo_object->mBVH.get_ins_root();
            if (ins_root != nullptr)
                if (ImGui::TreeNode("Insertion BVH Root"))
                {
                    ImGui::Text("Debug draw ");
                    ImGui::SameLine();
                    //node render options
                    if (ins_root->mShowMesh)
                    {
                        if (ImGui::SmallButton("Deactivate"))
                            ins_root->mShowMesh = false;
                    }
                    else
                    {
                        if (ImGui::SmallButton("Activate"))
                            ins_root->mShowMesh = true;
                    }
                    //ImGui::Checkbox("Debug lines", &root->mShowMesh);

                    show_bvh_nodes(ins_root, "Root");
                    ImGui::TreePop();
                }
        }
        //end of the window
        ImGui::End();
    }
}

void cs350::editor::show_bvh_nodes(cs350::BVH_TD_node* obj, std::string name)
{
    if (obj->mLeftNode != nullptr)
        if (ImGui::TreeNode((void*)(intptr_t)1, "Left node"))
        {
            ImGui::Text("Debug draw ");
            ImGui::SameLine();
            //node render options
            if (obj->mLeftNode->mShowMesh)
            {
                if (ImGui::SmallButton("Deactivate"))
                    obj->mLeftNode->mShowMesh = false;
            }
            else
            {
                if (ImGui::SmallButton("Activate"))
                    obj->mLeftNode->mShowMesh = true;
            }

            show_bvh_nodes(obj->mLeftNode, "Root");
            ImGui::TreePop();
        }

    if (obj->mLeftNode != nullptr)
        if (ImGui::TreeNode((void*)(intptr_t)2, "Right node"))
        {
            ImGui::Text("Debug draw ");
            ImGui::SameLine();
            //node render options
            if (obj->mRightNode->mShowMesh)
            {
                if (ImGui::SmallButton("Deactivate"))
                    obj->mRightNode->mShowMesh = false;
            }
            else
            {
                if (ImGui::SmallButton("Activate"))
                    obj->mRightNode->mShowMesh = true;
            }

            show_bvh_nodes(obj->mRightNode, "Root");
            ImGui::TreePop();
        }
}

void cs350::editor::show_bvh_nodes(cs350::BVH_node* obj, std::string name)
{
    if (obj->mLeftNode != nullptr)
        if (ImGui::TreeNode((void*)(intptr_t)1, "Left node"))
        {
            ImGui::Text("Debug draw ");
            ImGui::SameLine();
            //node render options
            if (obj->mLeftNode->mShowMesh)
            {
                if (ImGui::SmallButton("Deactivate"))
                    obj->mLeftNode->mShowMesh = false;
            }
            else
            {
                if (ImGui::SmallButton("Activate"))
                    obj->mLeftNode->mShowMesh = true;
            }

            show_bvh_nodes(obj->mLeftNode, "Root");
            ImGui::TreePop();
        }

    if (obj->mLeftNode != nullptr)
        if (ImGui::TreeNode((void*)(intptr_t)2, "Right node"))
        {
            ImGui::Text("Debug draw ");
            ImGui::SameLine();
            //node render options
            if (obj->mRightNode->mShowMesh)
            {
                if (ImGui::SmallButton("Deactivate"))
                    obj->mRightNode->mShowMesh = false;
            }
            else
            {
                if (ImGui::SmallButton("Activate"))
                    obj->mRightNode->mShowMesh = true;
            }

            show_bvh_nodes(obj->mRightNode, "Root");
            ImGui::TreePop();
        }
}

