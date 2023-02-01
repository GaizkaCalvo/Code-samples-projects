
#include "GLSLProgram.h" // GLSLProgram

#include <fstream>
#include <sstream>
#include <iostream>
#include <sys/stat.h>

#define DEBUG 1

#if DEBUG
static std::ostream & debug = std::cout;
#else
static std::ostringstream debug;
#endif // DEBUG

GLSLProgram * GLSLProgram::CreateShaderProgram(const std::string & vertShader, const std::string & fragShader)
{
    GLSLProgram * shaderProgram = new GLSLProgram();
    // First pass shader program
    if (!shaderProgram->CompileShaderFromFile(vertShader.c_str(), GLSLShader::VERTEX))
    {
        std::cout << vertShader << std::endl << shaderProgram->Log() << std::endl;
    }
    if (!shaderProgram->CompileShaderFromFile(fragShader.c_str(), GLSLShader::FRAGMENT))
    {
        std::cout << fragShader << std::endl << shaderProgram->Log() << std::endl;
    }

    if (!shaderProgram->Link())
    {
        std::cout << shaderProgram->Log() << std::endl;
    }

    return shaderProgram;
}

GLSLProgram::GLSLProgram() : handle_(0), linked_(false) {}

GLSLProgram::~GLSLProgram()
{
    if (handle_ > 0)
        glDeleteProgram(handle_);
}

bool GLSLProgram::CompileShaderFromFile(const char * fileName, GLSLShader::GLSLShaderType type)
{
    if (!FileExists(fileName))
    {
        log_string_ = "File not found.";
        return false;
    }

    if (handle_ <= 0)
    {
        handle_ = glCreateProgram();
        if (handle_ == 0)
        {
            log_string_ = "Unable to create shader program.";
            return false;
        }
    }

    std::ifstream inFile(fileName, std::ios::in);
    if (!inFile)
    {
        return false;
    }

    std::ostringstream code;
    while (inFile.good())
    {
        int c = inFile.get();
        if (!inFile.eof())
            code << (char)c;
    }
    inFile.close();

    return CompileShaderFromString(code.str(), type);
}

bool GLSLProgram::CompileShaderFromString(const std::string & source, GLSLShader::GLSLShaderType type)
{
    if (handle_ <= 0)
    {
        handle_ = glCreateProgram();
        if (handle_ == 0)
        {
            log_string_ = "Unable to create shader program.";
            return false;
        }
    }

    GLuint shaderhandle_ = 0;

    switch (type)
    {
        case GLSLShader::VERTEX:
            shaderhandle_ = glCreateShader(GL_VERTEX_SHADER);
            break;
        case GLSLShader::FRAGMENT:
            shaderhandle_ = glCreateShader(GL_FRAGMENT_SHADER);
            break;
        case GLSLShader::GEOMETRY:
            shaderhandle_ = glCreateShader(GL_GEOMETRY_SHADER);
            break;
        case GLSLShader::TESS_CONTROL:
            shaderhandle_ = glCreateShader(GL_TESS_CONTROL_SHADER);
            break;
        case GLSLShader::TESS_EVALUATION:
            shaderhandle_ = glCreateShader(GL_TESS_EVALUATION_SHADER);
            break;
        case GLSLShader::COMPUTE:
            shaderhandle_ = glCreateShader(GL_COMPUTE_SHADER);
            break;
        default:
            return false;
    }

    const char * c_code = source.c_str();
    glShaderSource(shaderhandle_, 1, &c_code, NULL);

    // Compile the shader
    glCompileShader(shaderhandle_);

    // Check for errors
    int result;
    glGetShaderiv(shaderhandle_, GL_COMPILE_STATUS, &result);
    if (GL_FALSE == result)
    {
        // Compile failed, store log and return false
        int length  = 0;
        log_string_ = "";
        glGetShaderiv(shaderhandle_, GL_INFO_LOG_LENGTH, &length);
        if (length > 0)
        {
            char * c_log   = new char[length];
            int    written = 0;
            glGetShaderInfoLog(shaderhandle_, length, &written, c_log);
            log_string_ = c_log;
            delete[] c_log;
        }

        return false;
    }
    else
    {
        // Compile succeeded, attach shader and return true
        glAttachShader(handle_, shaderhandle_);

        glDeleteShader(shaderhandle_);
        return true;
    }
}

bool GLSLProgram::Link()
{
    if (linked_)
        return true;
    if (handle_ <= 0)
        return false;

    glLinkProgram(handle_);

    int status = 0;
    glGetProgramiv(handle_, GL_LINK_STATUS, &status);
    if (GL_FALSE == status)
    {
        // Store log and return false
        int length  = 0;
        log_string_ = "";

        glGetProgramiv(handle_, GL_INFO_LOG_LENGTH, &length);

        if (length > 0)
        {
            char * c_log   = new char[length];
            int    written = 0;
            glGetProgramInfoLog(handle_, length, &written, c_log);
            log_string_ = c_log;
            delete[] c_log;
        }

        return false;
    }
    else
    {
        linked_ = true;
        return linked_;
    }
}

void GLSLProgram::Use() const
{
    if (handle_ <= 0 || (!linked_))
        return;
	//shaders
    glUseProgram(handle_);
}

std::string GLSLProgram::Log() const { return log_string_; }

int GLSLProgram::GetHandle() const { return handle_; }

bool GLSLProgram::IsLinked() const { return linked_; }

void GLSLProgram::SetUniform(const std::string & name, float val) const
{
    int loc = GetUniformLocation(name);

    if (loc >= 0)
    {
        glUniform1f(loc, val);
    }
    else
    {
        debug << "Uniform: " << name << " not found." << std::endl;
    }
}

void GLSLProgram::SetUniform(const std::string & name, int val) const
{
    int loc = GetUniformLocation(name);

    if (loc >= 0)
    {
        glUniform1i(loc, val);
    }
    else
    {
        debug << "Uniform: " << name << " not found." << std::endl;
    }
}

void GLSLProgram::SetUniform(const std::string & name, bool val) const
{
    int loc = GetUniformLocation(name);

    if (loc >= 0)
    {
        glUniform1i(loc, val);
    }
    else
    {
        debug << "Uniform: " << name << " not found." << std::endl;
    }
}

void GLSLProgram::SetUniform(const std::string & name, const glm::mat4 & m) const
{
	int loc = GetUniformLocation(name);
	if (loc >= 0)
	{
		glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
	}
	else
	{
		debug << "Uniform: " << name << " not found." << std::endl;
	}
}

void GLSLProgram::SetUniform(const std::string & name, const glm::vec3 & v) const
{
	int loc = GetUniformLocation(name);

	if (loc >= 0)
	{
		glUniform3f(loc, v.x, v.y, v.z);
	}
	else
	{
		debug << "Uniform: " << name << " not found." << std::endl;
	}
}

void GLSLProgram::SetUniform(const std::string & name, const glm::vec4 & v) const
{
	int loc = GetUniformLocation(name);

	if (loc >= 0)
	{
		glUniform4f(loc, v.x, v.y, v.z, v.w);
	}
	else
	{
		debug << "Uniform: " << name << " not found." << std::endl;
	}
}

void GLSLProgram::SetSubroutineUniform(const std::string & name, const std::string & funcName) const
{
    GLuint funcLoc = glGetSubroutineIndex(handle_, GL_FRAGMENT_SHADER, funcName.c_str());
    GLuint loc     = glGetSubroutineUniformLocation(handle_, GL_FRAGMENT_SHADER, name.c_str());

    if (loc >= 0)
    {

        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &funcLoc);
    }
    else
    {
        debug << "Uniform: " << name << " not found." << std::endl;
    }
}

int GLSLProgram::GetUniformLocation(const std::string & name) const
{
    return glGetUniformLocation(handle_, name.c_str());
}

bool GLSLProgram::FileExists(const std::string & fileName)
{
    struct stat info;
    int         ret = -1;

    ret = stat(fileName.c_str(), &info);
    return 0 == ret;
}
