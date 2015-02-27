#version 130

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

in vec3 VertexPosition;
in vec3 VertexColor;
in vec3 VertexNormal;
in vec2 VertexTexCoord;

out vec4 Position;
out vec4 Color;
out vec4 Normal;
out vec2 TexCoord;

out vec4 ReflectDirection;

void main()
{
	Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(VertexPosition, 1.0);
	gl_Position = Position;
	//Color = vec4(VertexColor, 1.0);
	Color = vec4(0.5, 0.5, 0.5, 1.0);
	mat4 NormalMatrix = transpose(inverse(ViewMatrix*ModelMatrix));
	Normal = normalize(NormalMatrix * vec4(VertexNormal, 1.0));
	ReflectDirection = vec4(VertexPosition, 1.0);
	TexCoord = VertexTexCoord;
}

