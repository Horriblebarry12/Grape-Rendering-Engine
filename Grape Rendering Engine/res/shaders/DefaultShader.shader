#shader vertex
#version 330 core
layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Normal;
layout(location = 2) in vec4 TexCoords; 

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec2 v_TexCoords;

void main()
{
	v_TexCoords = TexCoords.xy;
	gl_Position = u_Projection * u_View * u_Model * Position;
};


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoords;
uniform sampler2D TextureID;



void main()
{
	color = texture(TextureID, v_TexCoords);
}