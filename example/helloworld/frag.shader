#version 130
uniform vec3 LightPosition;
uniform vec3 LightColor;
uniform float La;
uniform float Ld;
uniform float Ls;

uniform sampler2D DiffuseTexture;

in vec4 Position;
in vec4 Normal;
in vec4 ReflectDirection;
in vec2 TexCoord;

out vec4 FragColor;

void main()
{
	vec3 LightDirection = normalize(LightPosition-Position.xyz);
	vec4 TexColor = texture2D(DiffuseTexture, TexCoord);
	FragColor = TexColor;
	//FragColor = min(dot(LightDirection, Normal.xyz), 1.0)*vec4(LightColor, 1.0);
	//FragColor = Position;

}

