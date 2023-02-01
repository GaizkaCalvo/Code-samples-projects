#version 400

out vec4 outputColor;

in vec2 ourTextCoord;
in vec3 aNormal;
in vec3 FragPos;
in mat3 TBN;

in vec3 Mode1;
in vec3 Mode2;
in vec3 Mode3;

uniform sampler2D textureData1;
uniform sampler2D textureData2;

struct LightSourceParameters
{
	int mLightType;

	vec4 mAmbientColor;
	vec4 mDiffuseColor;
	vec4 mSpecularColor;
	vec3 mPosition;
	vec3 mSpotDirection;

	float mSpotExponent;
	float mCutOff;
	float mOuterCutOff;

	float mConstantAttenuation;
	float mLinearAttenuation;
	float mQuadraticAttenuation;
};

struct MaterialParameters
{
	//k
	vec4 mMaterialAmbient;
	vec4 mMaterialDiffuse;
	vec4 mMaterialSpecular;
	//shininess
	float mMaterialShininess;
};

#define NR_POINT_LIGHTS 8

uniform LightSourceParameters LightSource[NR_POINT_LIGHTS];
uniform MaterialParameters Material;
uniform int LightAmount;
uniform vec3 viewPos;

uniform int Mode;
uniform mat4 ViewMat;
uniform mat4 ModelMat;

vec4 PointLight(vec4 outputColor, LightSourceParameters Light)
{
    // obtain normal from normal map in range [0,1]
	vec3 normal = textureLod(textureData2, vec2(ourTextCoord.x,1-ourTextCoord.y), 1 ).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
	//data that we will need to compute it in tangent space
    vec3 TangentLightPos = TBN * Light.mPosition;
    vec3 TangentViewPos  = TBN * viewPos;
    vec3 TangentFragPos  = TBN * FragPos;

	//ambient
	vec4 ambient = (Light.mAmbientColor * Material.mMaterialAmbient);

	// diffuse 
	//vec3 norm = normalize(aNormal);
	vec3 norm = normal;
	vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec4 diffuse = diff * (Light.mDiffuseColor * Material.mMaterialDiffuse);

	// specular
	vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), Material.mMaterialShininess);
	vec4 specular = spec * (Light.mSpecularColor * Material.mMaterialSpecular);

	// attenuation
	float distance    = length(Light.mPosition - FragPos);
	float attenuation = 1.0f / (Light.mConstantAttenuation + Light.mLinearAttenuation * distance +
																Light.mQuadraticAttenuation * (distance * distance));
	attenuation = clamp(attenuation, 0.0f, 1.0f);
	ambient  *= attenuation;  
	diffuse  *= attenuation;
	specular *= attenuation; 

	//vec4 result = (ambient + diffuse) * outputColor;
	vec4 result = (ambient + diffuse + specular) * outputColor;
	return result;
}

vec4 DirectionalLight(vec4 outputColor, LightSourceParameters Light)
{
    // obtain normal from normal map in range [0,1]
	vec3 normal = textureLod(textureData2, vec2(ourTextCoord.x,1-ourTextCoord.y), 1 ).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
	//data that we will need to compute it in tangent space
    vec3 TangentLightPos = TBN * Light.mPosition;
    vec3 TangentViewPos  = TBN * viewPos;
    vec3 TangentFragPos  = TBN * FragPos;
    vec3 TangentSpotDirection  = TBN * Light.mSpotDirection;

	//ambient
	vec4 ambient = (Light.mAmbientColor * Material.mMaterialAmbient);

	// diffuse 
	vec3 norm = normalize(normal);
	vec3  lightDir = normalize(-TangentSpotDirection);
	float diff = max(dot(norm, lightDir), 0.0);
	vec4 diffuse = (diff * Material.mMaterialDiffuse) * Light.mDiffuseColor;

	// specular
	vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), Material.mMaterialShininess);
	vec4 specular = spec * (Light.mSpecularColor * Material.mMaterialSpecular);

	vec4 result = (ambient + diffuse + specular) * outputColor;
	return result;
}

vec4 SpotLight(vec4 outputColor, LightSourceParameters Light)
{
    // obtain normal from normal map in range [0,1]
	vec3 normal = textureLod(textureData2, vec2(ourTextCoord.x,1-ourTextCoord.y), 1 ).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
	//data that we will need to compute it in tangent space
    vec3 TangentLightPos = TBN * Light.mPosition;
    vec3 TangentViewPos  = TBN * viewPos;
    vec3 TangentFragPos  = TBN * FragPos;

	//ambient
	vec4 ambient = (Light.mAmbientColor * Material.mMaterialAmbient);
	//outputColor *= ambient;

	// diffuse 
	//vec3 norm = normalize(aNormal);
	vec3 norm = normal;
	vec3 lightDir = normalize(TangentLightPos - TangentFragPos); 
	float diff = max(dot(norm, lightDir), 0.0);
	vec4 diffuse = diff * (Light.mDiffuseColor * Material.mMaterialDiffuse);

	// specular
	vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), Material.mMaterialShininess);
	vec4 specular = spec * (Light.mSpecularColor * Material.mMaterialSpecular);

	//SpotLight
	vec3 lightDir2 = normalize(Light.mPosition - FragPos); 
    float theta = dot(lightDir2, normalize(-Light.mSpotDirection)); 
    float epsilon = (Light.mCutOff - Light.mOuterCutOff);
    float intensity = clamp(pow((theta - Light.mOuterCutOff) / epsilon, Light.mSpotExponent), 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;


	// attenuation
	float distance    = length(Light.mPosition - FragPos);
	float attenuation = 1.0 / (Light.mConstantAttenuation + Light.mLinearAttenuation * distance +
																Light.mQuadraticAttenuation * (distance * distance));    
	attenuation = clamp(attenuation, 0.0f, 1.0f);
	ambient  *= attenuation;  
	diffuse  *= attenuation;
	specular *= attenuation; 

	vec4 result = (ambient + diffuse + specular) * outputColor;
	return result;
}


void main()
{
	if(Mode == 0)
	{
		outputColor = textureLod(textureData1, vec2(ourTextCoord.x,1-ourTextCoord.y), 1 ).rgba;

		vec4 result = vec4(0.0f);
		for(int i = 0; i < LightAmount; i++)
		{
			if(LightSource[i].mLightType == 0)
			{
				result += PointLight(outputColor, LightSource[i]);
			}
			if(LightSource[i].mLightType == 1)
			{
				result += DirectionalLight(outputColor, LightSource[i]);
			}
			if(LightSource[i].mLightType == 2)
			{
				result += SpotLight(outputColor, LightSource[i]);
			}
		}
		outputColor = result;
	}
	else
	{
		mat3 transformation = mat3(ViewMat * ModelMat);
		if(Mode == 1)
		{
			vec3 result = transpose(inverse(transformation)) * Mode1;
			result = normalize(result);
			outputColor = vec4(result, 1.0f);
		}
		if(Mode == 2)
		{
			vec3 result  = transformation * Mode2;
			result = normalize(result);
			outputColor = vec4(result, 1.0f);
		}
		if(Mode == 3)
		{
			vec3 result  = transformation * Mode3;
			result = normalize(result);
			outputColor = vec4(result, 1.0f);
		}
	}
}