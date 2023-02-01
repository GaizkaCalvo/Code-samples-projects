#version 400

layout(location = 0) in vec4 position;

uniform mat4 ModelMat;
uniform mat4 PerspectiveMat;
uniform mat4 ViewMat;

void main()
{
	gl_Position = PerspectiveMat * ViewMat * ModelMat * position;
}