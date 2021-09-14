#shader vertex
#version 330 core
layout(location = 0) in vec4 position;
out vec4 Position;
void main()
{
   gl_Position = position;
   Position = position;
};


#shader fragment
#version 330 core
in vec4 Position;
layout(location = 0) out vec4 color;

uniform vec4 u_ColorOffset;
void main()
{
	color = u_ColorOffset;
}