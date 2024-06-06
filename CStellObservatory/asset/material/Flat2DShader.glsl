//#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_ModelMatrix;

out vec2 v_TexCoord;

void main()
{
	gl_Position = u_ViewProjectionMatrix * u_ModelMatrix * vec4(a_Position, 1.0);
	v_TexCoord = a_TexCoord;
};

//#type fragment
#version 330 core

uniform vec4 u_Color;

in vec2 v_TexCoord;

out vec4 fragColor;

void main()
{
	fragColor = u_Color;
};