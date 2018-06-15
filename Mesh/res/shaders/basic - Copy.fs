#version 330 core

layout (location = 0) out vec4 frag_colour;

struct material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shine;
};

struct light
{
	vec4 position;
	vec4 diffuse;
	vec4 specular;
	float constantAttenuation, linearAttenuation, quadraticAttenuation;
};

vec4 sceneAmbient = vec4(0.2, 0.2, 0.2, 1.0);

uniform light l1;
uniform sampler2D uTexture;
uniform mat3 inv_transp;
uniform material mtl;

in vec3 normal;
in vec2 uv;
in vec4 worldPos;

void main()
{
	vec3 normalDir = normalize(inv_transp * normal);
	vec3 viewDir = normalize(vec3(-worldPos));
	vec3 lightDir;
	float attenuation;
	
	if(l1.position.w == 0.0)
	{
		attenuation = 1.0;
		lightDir = normalize(vec3(l1.position));
	}
	else
	{
		vec3 vertToLight = vec3(l1.position - worldPos);
		float distance = length(vertToLight);
		lightDir = normalize(vertToLight);
		attenuation = 1.0 / (l1.constantAttenuation 
			+ l1.linearAttenuation * distance
			+ l1.quadraticAttenuation * distance * distance);
	}
	
	vec4 texel = texture(uTexture, uv);
	
	vec3 ambient = vec3(mtl.ambient) * vec3(sceneAmbient);
	vec3 diffReflection = attenuation * vec3(l1.diffuse) * (vec3(mtl.diffuse)) * max(0.0, dot(normalDir, lightDir));
	vec3 specReflection;
	if(dot(normalDir, lightDir) < 0.0)
	{
		specReflection = vec3(0.0, 0.0, 0.0);
	}
	else
	{
		specReflection = attenuation * vec3(l1.specular) * vec3(mtl.specular) 
		* pow(max(0.0, dot(reflect(-lightDir, normalDir), viewDir)), mtl.shine);
	}
	
	frag_colour = vec4(ambient + diffReflection + specReflection, 1.0) * texel;
}