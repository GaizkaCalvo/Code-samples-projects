/**
 * @file fwd.hpp
 * @author Eder Beldad (eder.b1@digipen.edu)
 * @date 2020-06-01
 *
 * @copyright Copyright (c) 2020
 * @brief camera class that will store infomation and usefull functions
 */
#pragma once

#include "Helpers/math.hpp"
#include "Window/window.hpp"


namespace cs350 {
    class camera
    {
    public:
        void update();
        void create(const int w, const  int h);

        void set_position(glm::vec3 pos);
        void set_target(glm::vec3 target);
        void set_projection(float, glm::vec2 proj, float, float);

        glm::vec3 get_target() const;
        glm::vec3 get_position() const;

        // camera Attributes
        glm::vec3 mPosition;
        glm::vec3 mFront;
        glm::vec3 mUp;
        glm::vec3 mRight;
        glm::vec3 mWorldUp;
        // euler Angles
        float mYaw;
        float mPitch;
        // camera options
        float mMovementSpeed;
        float mMouseSensitivity;
        float mZoom;
        //camera view
        glm::mat4 mPerspective;
        //camera inputs
        bool mFirstMouse = false;
        double mLastX = 0.0;
        double mLastY = 0.0;

        // constructor with vectors
        camera(const glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), const  float yaw = -90.0f, const  float pitch = 0.0f);
        
        // constructor with scalar values
        camera(const float posX, const  float posY, const  float posZ, const  float upX, const  float upY, const  float upZ, const  float yaw,const float pitch);

        // returns the view matrix calculated using Euler Angles and the LookAt Matrix
        glm::mat4 get_view_matrix();
        // returns the perspective matrix calculated using Euler Angles and the LookAt Matrix
        glm::mat4 get_perspective_matrix();

        // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
        void process_mouse_scroll(const float yoffset);
        void proccess_inputs(GLFWwindow * Win);
        

    private:
        // calculates the front vector from the Camera's (updated) Euler Angles
        void updateCameraVectors();

        glm::vec3 mTarget;
    };
}