#version 400

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 TextCoord;
layout(location = 2) in vec3 Normal;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

out vec3 FragPos;
out vec2 ourTextCoord;
out vec3 aNormal;
out mat3 TBN;

out vec3 Mode1;
out vec3 Mode2;
out vec3 Mode3;

uniform mat4 ModelMat;
uniform mat4 PerspectiveMat;
uniform mat4 ViewMat;

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

    vec3 T = normalize(mat3(ModelMat) * aTangent);
    vec3 N = normalize(aNormal);
    vec3 B = normalize(mat3(ModelMat) * aBitangent);
    
    TBN = transpose(mat3(T, B, N));

	Mode1 = Normal;
	Mode2 = aTangent;
	Mode3 = aBitangent;
		
}