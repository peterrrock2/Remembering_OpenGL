#shader vertex
#version 330 core
layout(location = 0) in vec3 vertexPosition;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * vec4(vertexPosition, 1.0f);
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;


void main()
{
	color = vec4(1.0f); //makes a white cube
};