#version 120
struct PointLight
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct DirLight
{

	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

};
#define NR_POINT_LIGHTS 8
varying vec3 Normal;
varying vec3 FragPos;
varying vec2 TexCoords;

uniform float shininess;
uniform sampler2D mdiffuse1;
uniform sampler2D mspecular1;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirLight dirLight;
uniform vec3 viewPos;
uniform bool cube;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
void main()
{	
    float maxDist = 10.0f;
    float minDist = -10.0f;
    vec3 FogColor = vec3(.3, .3, 0.3);
    float dist = abs(FragPos.z);
    float fogFactor = (maxDist - dist) /
                      (maxDist - minDist);
    fogFactor = clamp( fogFactor, 0.0, 1.0 );
    //FogColor.xy *= TexCoords;
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);


    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
	result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);

    vec3 color = mix( FogColor, result, fogFactor );
    gl_FragColor = vec4(result, 1.0);
}
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    vec3 ambient = light.ambient * vec3(texture2D(mdiffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture2D(mdiffuse1, TexCoords));
    vec3 specular = light.specular * spec * (cube ? vec3(texture2D(mspecular1, TexCoords)) : 0.0f);
    return (ambient + diffuse + specular);
}
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    vec3 ambient = light.ambient * vec3(texture2D(mdiffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture2D(mdiffuse1, TexCoords));
    vec3 specular = light.specular * spec * (cube ? vec3(texture2D(mspecular1, TexCoords)) : 0.0f);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}