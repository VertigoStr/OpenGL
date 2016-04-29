#version 120

varying mat3 NormalMatrix;
varying vec2 tex;
varying vec3 eyeDir;
varying vec3 lightDir;
varying vec3 fragmentPosition;
varying float waterHeight;

uniform float time;
uniform float waterSpeed;
uniform mat2  projectionMatrixInverse; 
uniform sampler2D reflectionTexture;
uniform sampler2D WaterNormalMap;
uniform samplerCube environmentMap;

void main()
{           
    const vec3 waterMinColor = vec3(0.0, 0.05, 0.15);
    const vec3 waterMaxColor = vec3(0.0, 0.1,  0.15);
    const float distanceAttenuation = 2.0f;
    const vec4 lightSpecular = vec4(0.8, 0.8, 0.8, 0.0);
    const float waterTransparency = .5f;
    vec2 speed = vec2 ( 1.0f, 1.0f);
    speed *= waterSpeed;

    vec3 eyeDirNorm = normalize(eyeDir);
    vec3 normalNorm = 2.0 * ( texture2D(WaterNormalMap, tex).rgb - vec3(0.5) );

    float distVal = exp(-fragmentPosition.z * distanceAttenuation);
    normalNorm    = mix( vec3(0.0, 1.0, 0.0), normalNorm, pow(distVal, 10.0) );

    float dotValue   = dot(eyeDirNorm, normalNorm);
    vec3  waterColor = mix( waterMinColor, waterMaxColor, abs(dotValue) );

    float fresnel      = clamp( pow(1.0 + dotValue, 4.0), 0.05, 0.5 );

    vec4 reflectionColor = texture2D(reflectionTexture, tex);
    waterColor = mix(waterColor, reflectionColor.xyz * waterHeight, fresnel);
    waterColor = mix(waterColor, reflectionColor.xyz, fresnel);

    gl_FragColor = vec4(waterColor, 1.0f);
}