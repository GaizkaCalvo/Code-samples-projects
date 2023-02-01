#version 400

out vec4 outputColor;

in vec2 ourTextCoord;

void main()
{
	outputColor = vec4(ourTextCoord.x, ourTextCoord.y, 0.0f, 1.0f);
	return;
}
