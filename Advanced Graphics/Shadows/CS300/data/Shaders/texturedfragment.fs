#version 400

out vec4 outputColor;

in vec2 ourTextCoord;
in vec3 aNormal;
in vec3 FragPos;
in vec4 FragPosLightSpace;

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

uniform sampler2D shadowMap;

uniform int Mode;
uniform mat4 ViewMat;
uniform mat4 ModelMat;

vec4 PointLight(vec4 outputColor, LightSourceParameters Light)
{
	//ambient
	vec4 ambient = (Light.mAmbientColor * Material.mMaterialAmbient);

	// diffuse 
	vec3 norm = normalize(aNormal);
	vec3 lightDir = normalize(Light.mPosition - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec4 diffuse = diff * (Light.mDiffuseColor * Material.mMaterialDiffuse);

	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
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
	//ambient
	vec4 ambient = (Light.mAmbientColor * Material.mMaterialAmbient);

	// diffuse 
	vec3 norm = normalize(aNormal);
	vec3  lightDir = normalize(-Light.mSpotDirection);
	float diff = max(dot(norm, lightDir), 0.0);
	vec4 diffuse = (diff * Material.mMaterialDiffuse) * Light.mDiffuseColor;

	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), Material.mMaterialShininess);
	vec4 specular = spec * (Light.mSpecularColor * Material.mMaterialSpecular);

	vec4 result = (ambient + diffuse + specular) * outputColor;
	return result;
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
	// perform perspective divide
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	// transform to [0,1] range
	projCoords = projCoords * 0.5f + 0.5f;
	
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(aNormal);
    vec3 lightDir = normalize(LightSource[0].mPosition - FragPos);
    //float bias = max(0.05f * (1.0f - dot(normal, lightDir)), 0.005f);
    //float bias = 0.005f;
    float bias = 7.5f;

	float positionDepth = (fragPosLightSpace.z + bias) / fragPosLightSpace.w;

	//PCF
	float shadow = 0.0f;
    vec2 texelSize = 1.0f / textureSize(shadowMap, 0);
	int neigbours = 10;
	float count = 0;
    for(int x = -1; x <= neigbours; ++x)
    {
        for(int y = -1; y <= neigbours; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += pcfDepth - positionDepth > 0.0f  ? 1.0f : 0.0f;
			count++;
        }    
    }
    shadow /= count;

	return shadow;
}


vec4 SpotLight(vec4 outputColor, LightSourceParameters Light)
{
	//ambient
	vec4 ambient = (Light.mAmbientColor * Material.mMaterialAmbient);

	// diffuse 
	vec3 norm = normalize(aNormal);
	vec3 lightDir = normalize(Light.mPosition - FragPos); 
	float diff = max(dot(norm, lightDir), 0.0);
	vec4 diffuse = diff * (Light.mDiffuseColor * Material.mMaterialDiffuse);

	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
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

    // calculate shadow
    float shadow = ShadowCalculation(FragPosLightSpace);

	vec4 result = (ambient + shadow * (diffuse + specular)) * outputColor;
	//vec4 result = (ambient + (diffuse + specular)) * outputColor;
	return result;
}


void main()
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