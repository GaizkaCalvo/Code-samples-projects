#version 400

out vec4 outputColor;

in vec2 ourTextCoord;
in vec3 aNormal;
in vec3 FragPos;

uniform sampler2D textureData1;

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


vec4 PointLight(vec4 outputColor, LightSourceParameters Light)
{
	//ambient
	vec4 ambient = (Light.mAmbientColor * Material.mMaterialAmbient);
	//outputColor *= ambient;

	// diffuse 
	vec3 norm = normalize(aNormal);
	vec3 lightDir = normalize(Light.mPosition - FragPos); 
	float diff = max(dot(norm, lightDir), 0.0);
	vec4 diffuse = diff * (Light.mDiffuseColor * Material.mMaterialDiffuse);

	// specular
	float specularStrength = 1.0f;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), Material.mMaterialShininess);
	vec4 specular = specularStrength * spec * (Light.mSpecularColor * Material.mMaterialSpecular);

	// attenuation
	float distance    = length(Light.mPosition - FragPos);
	float attenuation = 1.0 / (Light.mConstantAttenuation + Light.mLinearAttenuation * distance +
																Light.mQuadraticAttenuation * (distance * distance));    
	ambient  *= attenuation;  
	diffuse  *= attenuation;
	specular *= attenuation; 

	vec4 result = (ambient + diffuse + specular) * outputColor;
	return result;
	//return vec4(norm, 1.0f);
}

vec4 DirectionalLight(vec4 outputColor, LightSourceParameters Light)
{
	//ambient
	vec4 ambient = (Light.mAmbientColor * Material.mMaterialAmbient);
	//outputColor *= ambient;

	// diffuse 
	vec3 norm = normalize(aNormal);
	vec3  lightDir = normalize(-Light.mSpotDirection);
	float diff = max(dot(norm, lightDir), 0.0);
	vec4 diffuse = (diff * Material.mMaterialDiffuse) * Light.mDiffuseColor;

	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), Material.mMaterialShininess);
	vec4 specular = (spec * Material.mMaterialSpecular) * Light.mSpecularColor ;

	vec4 result = (ambient + diffuse + specular) * outputColor;
	return result;
}

vec4 SpotLight(vec4 outputColor, LightSourceParameters Light)
{
	//ambient
	vec4 ambient = (Light.mAmbientColor * Material.mMaterialAmbient);
	//outputColor *= ambient;

	// diffuse 
	vec3 norm = normalize(aNormal);
	vec3 lightDir = normalize(Light.mPosition - FragPos); 
	float diff = max(dot(norm, lightDir), 0.0);
	vec4 diffuse = diff * (Light.mDiffuseColor * Material.mMaterialDiffuse);

	// specular
	float specularStrength = 1.0f;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), Material.mMaterialShininess);
	vec4 specular = specularStrength * spec * (Light.mSpecularColor * Material.mMaterialSpecular);

	//SpotLight
    float theta = dot(lightDir, normalize(-Light.mSpotDirection)); 
    float epsilon = (Light.mCutOff - Light.mOuterCutOff);
    float intensity = clamp(pow((theta - Light.mOuterCutOff) / epsilon, Light.mSpotExponent), 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;


	// attenuation
	float distance    = length(Light.mPosition - FragPos);
	float attenuation = 1.0 / (Light.mConstantAttenuation + Light.mLinearAttenuation * distance +
																Light.mQuadraticAttenuation * (distance * distance));    
	ambient  *= attenuation;  
	diffuse  *= attenuation;
	specular *= attenuation; 

	vec4 result = (ambient + diffuse + specular) * outputColor;
	return result;
}

void main()
{
	outputColor = vec4(ourTextCoord.x, ourTextCoord.y, 0.0f, 1.0f);
	
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
