#version 330 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aColor;

uniform vec4 ourPos;
out vec3 ourColor;
out vec4 vertPos;

void main()
{
	//gl_Position = vec4(aPos, 1.0);
	gl_Position = vec4(aPos.x + ourPos.x, -aPos.y, aPos.z, 1.0);
	ourColor = aColor;
	vertPos = gl_Position;
}

