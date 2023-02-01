#include "Helpers/pch.hpp"
#include "shader.hpp"
#include "Helpers/opengl.hpp"

namespace cs350 {
    /**
    * Shader creation
    * @param vertShader string to create vs
    * @param fragShader string to create fs
    * @return the shader if we created one
    */
    shader* shader::create_shader_program(const std::string& vertShader, const std::string& fragShader)
    {
        shader* shaderProgram = new shader();
        // First pass shader program
        if (!shaderProgram->compile_shader_from_file(vertShader.c_str(), shader_type::VERTEX))
        {
            std::cout << vertShader << std::endl << shaderProgram->Log() << std::endl;
        }
        if (!shaderProgram->compile_shader_from_file(fragShader.c_str(), shader_type::FRAGMENT))
        {
            std::cout << fragShader << std::endl << shaderProgram->Log() << std::endl;
        }
        //check if its linked
        if (!shaderProgram->link())
        {
            std::cout << "Error lLoading the shader: ";
            std::cout << shaderProgram->Log() << std::endl;
        }
        //return
        return shaderProgram;
    }
    /**
    * shader constructor
    */
    shader::shader() : mHandle(0), mLinked(false)
    {
    }
    /**
    * shader destructor
    */
    shader::~shader()
    {
        //delete the handle if we need to 
        if (mHandle > 0)
            glDeleteProgram(mHandle);
    }
    /**
    * We compile the shader from the file given
    * @fileName name of the file
    * @type type of shader that we got
    */
    bool shader::compile_shader_from_file(const char* fileName, cs350::shader_type type)
    {
        //create the handle if we need to 
        if (mHandle <= 0)
        {
            mHandle = glCreateProgram();
            if (mHandle == 0)
            {
                mLogString = "Unable to create shader program.";
                return false;
            }
        }
        //open de file
        std::ifstream inFile(fileName, std::ios::in);
        if (!inFile)
        {
            return false;
        }
        if (!inFile.good())
        {
            mLogString = "File was not opened correctly or doesn't exist.";
            return false;
        }
        //check if the file was opened correctly
        std::ostringstream code;
        while (inFile.good())
        {
            int c = inFile.get();
            if (!inFile.eof())
                code << (char)c;
        }
        //close file
        inFile.close();
        //compile the shader
        return compile_shader_from_string(code.str(), type);
    }
    /**
    * we create the shader from what we got from the string
    * @param source data of the shader
    * @param type type of the shader
    */
    bool shader::compile_shader_from_string(const std::string& source, cs350::shader_type type)
    {
        //check handle
        if (mHandle <= 0)
        {
            mHandle = glCreateProgram();
            if (mHandle == 0)
            {
                mLogString = "Unable to create shader program.";
                return false;
            }
        }

        GLuint shaderhandle_ = 0;
        //create the shader depending on what we got 
        switch (type)
        {
        case shader_type::VERTEX:
            shaderhandle_ = glCreateShader(GL_VERTEX_SHADER);
            break;
        case shader_type::FRAGMENT:
            shaderhandle_ = glCreateShader(GL_FRAGMENT_SHADER);
            break;
        case shader_type::GEOMETRY:
            shaderhandle_ = glCreateShader(GL_GEOMETRY_SHADER);
            break;
        case shader_type::TESS_CONTROL:
            shaderhandle_ = glCreateShader(GL_TESS_CONTROL_SHADER);
            break;
        case shader_type::TESS_EVALUATION:
            shaderhandle_ = glCreateShader(GL_TESS_EVALUATION_SHADER);
            break;
        case shader_type::COMPUTE:
            shaderhandle_ = glCreateShader(GL_COMPUTE_SHADER);
            break;
        default:
            return false;
        }

        const char* c_code = source.c_str();
        glShaderSource(shaderhandle_, 1, &c_code, NULL);

        // Compile the shader
        glCompileShader(shaderhandle_);

        // Check for errors
        int result;
        glGetShaderiv(shaderhandle_, GL_COMPILE_STATUS, &result);
        if (GL_FALSE == result)
        {
            // Compile failed, store log and return false
            int length = 0;
            mLogString = "";
            glGetShaderiv(shaderhandle_, GL_INFO_LOG_LENGTH, &length);
            if (length > 0)
            {
                char* c_log = new char[length];
                int    written = 0;
                glGetShaderInfoLog(shaderhandle_, length, &written, c_log);
                mLogString = c_log;
                delete[] c_log;
            }

            return false;
        }
        else
        {
            // Compile succeeded, attach shader and return true
            glAttachShader(mHandle, shaderhandle_);

            glDeleteShader(shaderhandle_);
            return true;
        }
    }
    /**
    * We link the shader
    * @return true if we got it, false otherwise
    */
    bool shader::link()
    {
        //check some stats
        if (mLinked)
            return true;
        if (mHandle <= 0)
            return false;

        glLinkProgram(mHandle);

        int status = 0;
        glGetProgramiv(mHandle, GL_LINK_STATUS, &status);
        if (GL_FALSE == status)
        {
            // Store log and return false
            int length = 0;
            mLogString = "";

            glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &length);

            if (length > 0)
            {
                char* c_log = new char[length];
                int    written = 0;
                glGetProgramInfoLog(mHandle, length, &written, c_log);
                mLogString = c_log;
                delete[] c_log;
            }

            return false;
        }
        else
        {
            mLinked = true;
            return mLinked;
        }
    }
    /**
    * function to use the shader stored
    */
    void shader::use() const
    {
        if (mHandle <= 0 || (!mLinked))
            return;
        //shaders
        glUseProgram(mHandle);
    }
    /**
	* We get the log 
	* @return Log value
	*/
    std::string shader::Log() const { return mLogString; }

    /**
    * We get the handle
    * @return Handle value
    */
    int shader::handle() const { return mHandle; }
    /**
    * We check if its linked
    * @return Linked value
    */
    bool shader::is_linked() const { return mLinked; }
    /**
    * uniform that we will set in the shader
    * @param name of the uniform
    * @param val uniform we set
    */
    void shader::set_uniform(const std::string& name, float val) const
    {
        int loc = get_uniform_location(name);

        if (loc >= 0)
        {
            glUniform1f(loc, val);
        }
        else
        {
            std::cout << "Uniform: " << name << " not found." << std::endl;
        }
    }
    /**
   * uniform that we will set in the shader
   * @param name of the uniform
   * @param val uniform we set
   */
    void shader::set_uniform(const std::string& name, int val) const
    {
        int loc = get_uniform_location(name);

        if (loc >= 0)
        {
            glUniform1i(loc, val);
        }
        else
        {
            std::cout << "Uniform: " << name << " not found." << std::endl;
        }
    }
    /**
   * uniform that we will set in the shader
   * @param name of the uniform
   * @param val uniform we set
   */
    void shader::set_uniform(const std::string& name, bool val) const
    {
        int loc = get_uniform_location(name);

        if (loc >= 0)
        {
            glUniform1i(loc, val);
        }
        else
        {
            std::cout << "Uniform: " << name << " not found." << std::endl;
        }
    }
    /**
   * uniform that we will set in the shader
   * @param name of the uniform
   * @param val uniform we set
   */
    void shader::set_uniform(const std::string& name, const glm::mat4& m) const
    {
        int loc = get_uniform_location(name);
        if (loc >= 0)
        {
            glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
        }
        else
        {
            std::cout << "Uniform: " << name << " not found." << std::endl;
        }
    }
    void shader::set_uniform(const std::string& name, const glm::mat3& m) const
    {
        int loc = get_uniform_location(name);
        if (loc >= 0)
        {
            glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
        }
        else
        {
            std::cout << "Uniform: " << name << " not found." << std::endl;
        }
    }
    /**
   * uniform that we will set in the shader
   * @param name of the uniform
   * @param val uniform we set
   */
    void shader::set_uniform(const std::string& name, const glm::vec3& v) const
    {
        int loc = get_uniform_location(name);

        if (loc >= 0)
        {
            glUniform3f(loc, v.x, v.y, v.z);
        }
        else
        {
            std::cout << "Uniform: " << name << " not found." << std::endl;
        }
    }
    /**
   * uniform that we will set in the shader
   * @param name of the uniform
   * @param val uniform we set
   */
    void shader::set_uniform(const std::string& name, const glm::vec4& v) const
    {
        int loc = get_uniform_location(name);

        if (loc >= 0)
        {
            glUniform4f(loc, v.x, v.y, v.z, v.w);
        }
        else
        {
            std::cout << "Uniform: " << name << " not found." << std::endl;
        }
    }
    /**
   * subrutineuniform that we will set in the shader
   * @param name of the uniform
   * @param funcName function name that we set
   */
    void shader::set_subroutine_uniform(const std::string& name, const std::string& funcName) const
    {
        GLuint funcLoc = glGetSubroutineIndex(mHandle, GL_FRAGMENT_SHADER, funcName.c_str());
        GLuint loc = glGetSubroutineUniformLocation(mHandle, GL_FRAGMENT_SHADER, name.c_str());

        if (loc >= 0)
        {

            glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &funcLoc);
        }
        else
        {
            std::cout << "Uniform: " << name << " not found." << std::endl;
        }
    }
    /**
    * we set the uniform location
    * @param name of the uniform
    * @param funcName function name that we set
    */
    int shader::get_uniform_location(const std::string& name) const
    {
        return glGetUniformLocation(mHandle, name.c_str());
    }
}