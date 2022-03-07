#shader vertex
#version 330 core
layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Normal;
layout(location = 2) in vec4 TexCoords; 
uniform mat4 u_MVP;

out vec4 v_Normal;

void main()
{
	v_Normal = Normal;
	gl_Position = u_MVP * Position;
};


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Normal;

void main()
{
	color = vec4((v_Normal.x+1.0)/2.0, (v_Normal.y + 1.0) / 2.0, (v_Normal.z + 1.0) / 2.0, 1.0);
}