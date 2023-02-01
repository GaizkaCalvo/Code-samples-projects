#ifndef GLSLPROGRAM_H
#define GLSLPROGRAM_H

#include <GL/glew.h>
#include <GL/gl.h>

#include <string>

#include <glm/glm.hpp>
#include "Light/Light.h"

namespace GLSLShader
{
enum GLSLShaderType
{
    VERTEX,
    FRAGMENT,
    GEOMETRY,
    TESS_CONTROL,
    TESS_EVALUATION,
    COMPUTE
};
};

/*
 * @brief   Class encapsulating a GLSL Program
 *
 * @detail  Class encapsulating a GLSL Program that also adds some member functions to set uniform variables.
 * This class was mostly copied from the Cookbook code.
 */
class GLSLProgram
{
  public:
    static GLSLProgram * CreateShaderProgram(const std::string & vertShader,
                                             const std::string & fragShader);

    GLSLProgram();
    ~GLSLProgram();

    bool CompileShaderFromFile(const char * fileName, GLSLShader::GLSLShaderType type);
    bool CompileShaderFromString(const std::string & source, GLSLShader::GLSLShaderType type);
    bool Link();
    void Use() const;

    std::string Log() const;

    int  GetHandle() const;
    bool IsLinked() const;

    void SetUniform(const std::string & name, float val) const;
    void SetUniform(const std::string & name, int val) const;
    void SetUniform(const std::string & name, bool val) const;
	void SetUniform(const std::string & name, const glm::mat4 & m) const;
	void SetUniform(const std::string & name, const glm::vec3 & v) const;
	void SetUniform(const std::string & name, const glm::vec4 & v) const;

    void SetSubroutineUniform(const std::string & name, const std::string & funcName) const;

  private:
    int  GetUniformLocation(const std::string & name) const;
    bool FileExists(const std::string & fileName);

    int         handle_;
    bool        linked_;
    std::string log_string_;
};

#endif // GLSLPROGRAM_H