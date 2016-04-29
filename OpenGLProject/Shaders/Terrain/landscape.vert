#version 120
uniform mat4 view;
uniform mat4 proj;
uniform mat4 model;

attribute vec3 inPos;
attribute vec3 inNormal;
attribute vec2 inTex;

varying vec2 texCoord;
varying vec4 normal;
void main()
{
	gl_Position = proj * view * model * vec4(inPos, 1.0);
	texCoord = inTex;
	normal.xyz = inNormal;
	normal.w = inPos.y;
}