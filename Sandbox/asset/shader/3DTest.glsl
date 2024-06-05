//#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

out vec3 v_Normal;
out vec2 v_TexCoord;
out vec3 v_LightPosition;
out vec3 v_CameraPosition;
out vec3 v_ReflectedDir;

uniform mat4 u_MVP;
uniform mat4 u_ModelView;
uniform vec3 u_LightPosition;
uniform vec3 u_CameraPosition;

void main()
{
	v_TexCoord = a_TexCoord;
	v_Normal = normalize(mat3(transpose(inverse(u_ModelView))) * a_Normal);
	v_LightPosition = normalize(u_LightPosition);
	v_CameraPosition = normalize(u_CameraPosition);
	v_ReflectedDir = reflect(-v_LightPosition, v_Normal);
	gl_Position = u_MVP * vec4(a_Position, 1.0);
};

//#type fragment
#version 330 core

layout(location = 0) out vec4 FragColor;

in vec3 v_Normal;
in vec2 v_TexCoord;
in vec3 v_LightPosition;
in vec3 v_CameraPosition;
in vec3 v_ReflectedDir;

uniform sampler2D u_Texture;

uniform vec3 u_AmbientLight;
uniform float u_LightIntensity;
uniform vec4 u_LightColor;
uniform vec3 u_SpecularColor;
uniform float u_SpecularAlpha;

void main()
{
	vec3 normal = normalize(v_Normal);
	vec3 reflected = normalize(v_Normal);
	float diffuse = max(dot(normal, v_LightPosition), 0.0);
	float phong = max(dot(reflected, v_CameraPosition), 0.0);
	vec3 halfAngle = (v_LightPosition + v_CameraPosition) / length(v_LightPosition + v_CameraPosition);
	float blinn = max(dot(normal, halfAngle), 0.0);

	//FragColor = vec4(1.0, 0.0, 0.0, 1.0) + (u_LightIntensity * vec4(u_SpecularColor, 1.0) * pow(blinn, u_SpecularAlpha)) + vec4(u_AmbientLight, 1.0);
	//FragColor = (u_LightIntensity * vec4(u_SpecularColor, 1.0) * pow(phong, u_SpecularAlpha)) + vec4(u_AmbientLight, 1.0);
	//FragColor = vec4(1.0, 1.0, 0.0, 1.0) * (u_LightIntensity * ((diffuse * u_LightColor) + ((vec4(u_SpecularColor, 1.0) * pow(blinn, u_SpecularAlpha)))) + vec4(u_AmbientLight, 1.0));
	FragColor = texture(u_Texture, v_TexCoord) * (u_LightIntensity * ((diffuse * u_LightColor) + ((vec4(u_SpecularColor, 1.0) * pow(blinn, u_SpecularAlpha)))) + vec4(u_AmbientLight, 1.0));
	//FragColor = vec4(0.1, 0.1, 0.1, 1.0) * (u_LightIntensity * ((diffuse * u_LightColor) + ((vec4(u_SpecularColor, 1.0) * pow(blinn, u_SpecularAlpha)))) + vec4(u_AmbientLight, 1.0));
	//FragColor = vec4(v_TexCoord, 0.0, 1.0);
	//FragColor = vec4(1.0, 1.0, 0.0, 1.0) * ((u_LightIntensity * diffuse * u_LightColor) + vec4(u_AmbientLight, 1.0));
	//FragColor = vec4(v_Normal, 1.0f);
};