#shader vertex
#version 330 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 TextCoord;
out vec2 v_textCoord;
void main()
{
	v_textCoord = vec2(TextCoord.xy); //vec2(position.x + 0.5, position.y + 0.5);
	gl_Position = position;
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