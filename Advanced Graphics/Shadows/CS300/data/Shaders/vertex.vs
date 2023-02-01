#version 400

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 TextCoord;
layout(location = 2) in vec3 Normal;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

out vec2 ourTextCoord;

uniform mat4 ModelMat;
uniform mat4 PerspectiveMat;
uniform mat4 ViewMat;

void main()
{
	gl_Position = PerspectiveMat * ViewMat * ModelMat * position;
	ourTextCoord = TextCoord;
}