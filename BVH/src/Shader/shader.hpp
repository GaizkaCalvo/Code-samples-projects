/**
 * @file shader.hpp
 * @author Gaizka Calvo (gaizka.c@digipen.edu)
 * @date 21/09/2020
 *
 * @copyright Copyright (c) 2020
 * @brief class that will take control of the shaders with some functions
 */
#pragma once

namespace cs350 {

    enum class shader_type
    {
        VERTEX,
        FRAGMENT,
        GEOMETRY,
        TESS_CONTROL,
        TESS_EVALUATION,
        COMPUTE
    };

    /*
     * @brief   Class encapsulating a GLSL Program
     *
     * @detail  Class encapsulating a GLSL Program that also adds some member functions to set uniform variables.
     * This class was mostly copied from the Cookbook code.
     */
    class shader
    {
    public:
        static shader* create_shader_program(const std::string& vertShader,
            const std::string& fragShader);

        shader();
        ~shader();

        bool compile_shader_from_file(const char* fileName, cs350::shader_type type);
        bool compile_shader_from_string(const std::string& source, cs350::shader_type type);
        bool link();
        void use() const;

        std::string Log() const;

        int  handle() const;
        bool is_linked() const;

        void set_uniform(const std::string& name, float val) const;
        void set_uniform(const std::string& name, int val) const;
        void set_uniform(const std::string& name, bool val) const;
        void set_uniform(const std::string& name, const glm::mat4& m) const;
        void set_uniform(const std::string& name, const glm::vec3& v) const;
        void set_uniform(const std::string& name, const glm::vec4& v) const;

        void set_subroutine_uniform(const std::string& name, const std::string& funcName) const;

    private:
        int  get_uniform_location(const std::string& name) const;

        int         mHandle;
        bool        mLinked;
        std::string mLogString;
    };
}