#shader vertex
#version 330 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 TextCoord;
out vec2 v_textCoord;

uniform mat4 u_MVP;

void main()
{
	v_textCoord = TextCoord;
	gl_Position = u_MVP * position ;
};


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform sampler2D u_Texture;
in vec2 v_textCoord;

void main()
{
	vec4 TexColor = texture(u_Texture, v_textCoord);
	color = TexColor;
}