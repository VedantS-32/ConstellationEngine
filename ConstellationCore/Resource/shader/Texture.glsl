//#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;
layout(location = 5) in int a_EntityID;

uniform mat4 u_ViewProjectionMatrix;

out vec4 v_Color;
out vec2 v_TexCoord;
flat out float v_TexIndex;
out float v_TilingFactor;
out flat int v_EntityID;

void main()
{
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;
	v_TilingFactor = a_TilingFactor;
	v_EntityID = a_EntityID;

	gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
}

//#type fragment
#version 450 core

uniform vec4 u_Tint;
uniform float u_Tiling;
uniform sampler2D u_Texture[32];

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
in float v_TilingFactor;
in flat int v_EntityID;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out int entityID;

void main()
{
	fragColor = texture(u_Texture[int(v_TexIndex)], v_TexCoord * v_TilingFactor) * v_Color;
	entityID = v_EntityID;
}