#version 400

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 AverageNormal;
layout(location = 2) in vec2 TextCoord;
layout(location = 3) in vec3 Normal;

out vec2 ourTextCoord;
out vec3 aNormal;
out vec3 FragPos;

uniform mat4 ModelMat;
uniform mat4 PerspectiveMat;
uniform mat4 ViewMat;

uniform bool mAverage;

void main()
{
	gl_Position = PerspectiveMat * ViewMat * ModelMat * position;
	ourTextCoord = TextCoord;

	//lighting computations
    FragPos = vec3(ModelMat * position);

	if(!mAverage)
		aNormal = mat3(transpose(inverse(ModelMat))) * Normal;
	else
		aNormal = mat3(transpose(inverse(ModelMat))) * AverageNormal;
}