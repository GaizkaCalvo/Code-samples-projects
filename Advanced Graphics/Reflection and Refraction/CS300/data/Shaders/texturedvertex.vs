#version 400

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 TextCoord;
layout(location = 2) in vec3 Normal;

out vec3 FragPos;
out vec2 ourTextCoord;
out vec3 aNormal;
out vec4 FragPosLightSpace;

uniform mat4 ModelMat;
uniform mat4 PerspectiveMat;
uniform mat4 ViewMat;
uniform mat4 lightSpaceMatrix;

uniform bool mAverage;
uniform int LightAmount;

void main()
{
	gl_Position = PerspectiveMat * ViewMat * ModelMat * position;
	ourTextCoord = TextCoord;

	if(LightAmount == -1)
		return;

	//lighting computations
    FragPos = vec3(ModelMat * position);

	aNormal = mat3(transpose(inverse(ModelMat))) * Normal;

	FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0f);
		
}