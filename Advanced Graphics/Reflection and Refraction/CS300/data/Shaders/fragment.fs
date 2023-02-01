#version 400

out vec4 outputColor;

in vec2 ourTextCoord;

uniform sampler2D textureData1;

void main()
{
	outputColor = textureLod(textureData1, vec2(ourTextCoord.x,1-ourTextCoord.y), 1 ).rgba;
}