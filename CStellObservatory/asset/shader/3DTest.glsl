//#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

out vec3 v_Normal;
out vec2 v_TexCoord;
out vec3 v_LightPosition;
out vec3 v_CameraPosition;
out vec3 v_ReflectedDir;
out int v_EntityID;

uniform int u_EntityID;
uniform mat4 u_Model;
uniform mat4 u_ModelView;
uniform vec3 u_LightPosition;
uniform mat4 u_ViewProjection;
uniform vec3 u_CameraPosition;

//layout(std140) uniform ModelCommons
//{
//};

void main()
{
	mat4 MVP = u_ViewProjection * u_Model;
	vec3 u_LightPosition = vec3(1.0f, 1.0f, 1.0f);
	v_TexCoord = a_TexCoord;
	v_EntityID = u_EntityID;
	v_Normal = normalize(mat3(transpose(inverse(u_ModelView))) * a_Normal);
	v_LightPosition = normalize(u_LightPosition);
	v_CameraPosition = normalize(u_CameraPosition);
	v_ReflectedDir = reflect(-v_LightPosition, v_Normal);
	gl_Position = MVP * vec4(a_Position, 1.0);
};

//#type fragment
#version 450 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out int EntityID;

in vec3 v_Normal;
in vec2 v_TexCoord;
in vec3 v_LightPosition;
in vec3 v_CameraPosition;
in vec3 v_ReflectedDir;
in flat int v_EntityID;

uniform sampler2D u_Texture;

//uniform vec3 u_SpecularColor;

layout(std140) uniform ModelProps
{
	uniform float Tiling;
	//uniform vec4 Color;
	uniform vec4 u_LightColor;
	uniform vec3 u_AmbientLight;
	uniform float u_LightIntensity;
	uniform float u_SpecularAlpha;
};

void main()
{
	//vec3 u_AmbientLight = vec3(0.05f, 0.05f, 0.2f);
	//float u_LightIntensity = 1.0f;
	//vec4 u_LightColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec3 u_SpecularColor = vec3(1.0f, 1.0f, 1.0f);
	//float u_SpecularAlpha = 100.0f;

	vec3 normal = normalize(v_Normal);
	vec3 reflected = normalize(v_Normal);
	float diffuse = max(dot(normal, v_LightPosition), 0.0);
	float phong = max(dot(reflected, v_CameraPosition), 0.0);
	vec3 halfAngle = (v_LightPosition + v_CameraPosition) / length(v_LightPosition + v_CameraPosition);
	float blinn = max(dot(normal, halfAngle), 0.0);

	//FragColor = vec4(1.0, 0.0, 0.0, 1.0) + (u_LightIntensity * vec4(u_SpecularColor, 1.0) * pow(blinn, u_SpecularAlpha)) + vec4(u_AmbientLight, 1.0);
	//FragColor = (u_LightIntensity * vec4(u_SpecularColor, 1.0) * pow(phong, u_SpecularAlpha)) + vec4(u_AmbientLight, 1.0);
	//FragColor = vec4(1.0, 1.0, 0.0, 1.0) * (u_LightIntensity * ((diffuse * u_LightColor) + ((vec4(u_SpecularColor, 1.0) * pow(blinn, u_SpecularAlpha)))) + vec4(u_AmbientLight, 1.0));
	FragColor = texture(u_Texture, v_TexCoord * Tiling) * (u_LightIntensity * ((diffuse * u_LightColor) + ((vec4(u_SpecularColor, 1.0) * pow(blinn, u_SpecularAlpha)))) + vec4(u_AmbientLight, 1.0));
	//FragColor = vec4(0.1, 0.1, 0.1, 1.0) * (u_LightIntensity * ((diffuse * u_LightColor) + ((vec4(u_SpecularColor, 1.0) * pow(blinn, u_SpecularAlpha)))) + vec4(u_AmbientLight, 1.0));
	//FragColor = (texture(u_Texture, v_TexCoord * Tiling) * (u_LightIntensity * ((diffuse * u_LightColor)))) + vec4(u_AmbientLight, 1.0f);
	//FragColor = Color * ((u_LightIntensity * diffuse * u_LightColor) + vec4(u_AmbientLight, 1.0));
	//FragColor = vec4(v_Normal, 1.0f);
	//FragColor = (Color * diffuse * u_LightIntensity * u_LightColor) + vec4(u_AmbientLight, 1.0f);

	EntityID = v_EntityID;
};