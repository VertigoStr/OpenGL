#version 120
attribute vec3 position;
attribute vec3 inNormal;
attribute vec2 texCoords;

varying vec3 Normal;
varying vec3 FragPos;
varying vec2 TexCoords;

uniform mat4 model;
uniform mat4 mvp;
uniform mat3 TIM;
void main()
{
	gl_Position = mvp * vec4(position, 1.0f);
	FragPos = vec3(model * vec4(position, 1.0f));
	Normal = TIM * inNormal;
	TexCoords = texCoords;
}