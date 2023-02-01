#version 400

out vec4 outputColor;

uniform vec3 Color;

void main()
{
	//if(Color.r > 0.5f)
		outputColor = vec4(Color, 1.0f);
	//else
	//	discard;
	
}
