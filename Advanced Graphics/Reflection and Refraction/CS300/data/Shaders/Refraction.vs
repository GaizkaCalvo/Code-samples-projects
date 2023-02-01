#version 400

layout(location = 0) in vec4 aPos;
layout(location = 1) in vec2 TextCoord;
layout(location = 2) in vec3 aNormal;

out vec3 Normal;
out vec3 Position;

uniform mat4 ModelMat;
uniform mat4 ViewMat;
uniform mat4 PerspectiveMat;

void main()
{
    Normal = mat3(transpose(inverse(ModelMat))) * aNormal;
    Position = vec3(ModelMat * aPos);
    gl_Position = PerspectiveMat * ViewMat * ModelMat * aPos;
}