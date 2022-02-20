#shader vertex
#version 330 core
layout(location = 0) in vec4 position;

out vec2 v_Pos;

void main()
{
    v_Pos = position.xy + vec2(0.5);
	gl_Position = position;
};


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
in vec2 v_Pos;

uniform float iTime;

vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

// All components are in the range [0…1], including hue.
vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{
    vec3 col = 0.5 + 0.5 * cos(iTime + v_Pos.xyx + vec3(0, 2, 4));

	color = vec4(col, 1.0);

    // hsv2rgb(vec3(abs(cos(iTime)), 1.0, 1.0))
}