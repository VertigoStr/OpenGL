#version 120
attribute vec3 position;
attribute vec3 normal;
attribute vec2 texCoords;

varying mat3 NormalMatrix;
varying vec2 tex;
varying vec3 eyeDir;
varying vec3 lightDir;
varying vec3 fragmentPosition;
varying float waterHeight;

uniform mat4 model;
uniform vec3 cameraPos;
uniform mat4 view;
uniform mat4 projection;
uniform float time;
uniform float waterSpeed;
uniform mat3 normalMatrix;
uniform sampler2D heightMap;

void main()
{
    vec2 speed = vec2 ( 1.0f, 1.0f);
    speed *= waterSpeed;
    float sharpness = 0.5f;
    float amplitude = 2.0f;
    vec3 lightPosition = vec3(127, 127, 127);
    vec3 pos = position;    

    vec4 texel = texture2D(heightMap, 0.5 * texCoords + speed * time); 
    float attenuation = min( pow((1.0 - pos.y), 0.2), pow(1.0 - abs(pos.x - 0.5) * 2.0, 1.2) );

    pos.y = texel.x * attenuation * amplitude;
    pos.xz -= sharpness * texel.zw * attenuation;

    gl_Position = projection * view * model * vec4(pos, 1.0f);

    eyeDir   = pos.xyz - cameraPos.xyz;
    lightDir = lightPosition.xyz;

    waterHeight = pos.y;
    NormalMatrix = normalMatrix;
    fragmentPosition = vec3(gl_Position.xy / gl_Position.w, gl_Position.w);
    tex = texCoords;
}  