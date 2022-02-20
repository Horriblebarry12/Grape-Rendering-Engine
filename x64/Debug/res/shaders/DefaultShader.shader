#shader vertex
#version 330 core
layout(location = 0) in vec4 position;
uniform mat4 u_MVP;

out vec4 v_Position;

void main()
{
	v_Position = u_MVP * position;
	gl_Position = u_MVP * position;
};


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Position;

void main()
{
	color = vec4(v_Position.x, v_Position.y, (v_Position.z + 2)/2, 1.0);
}