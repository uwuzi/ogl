#version 330 core

out vec4 FragColor;

//in vec4 vertPos;
//uniform vec4 ourColor;

in vec3 ourColor;
in vec2 texCoord;

uniform sampler2D  ourTexture;


void main()
{
	//FragColor = vec4(ourColor, 1.0);
	//FragColor = vertPos;
	FragColor = texture(ourTexture, texCoord) * vec4(ourColor,1.0);
}