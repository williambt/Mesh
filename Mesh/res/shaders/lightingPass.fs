#version 330 core

layout (location = 0) out vec4 FragColour;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColour;

struct light
{
	vec4 position;
	vec4 diffuse;
	vec4 specular;
	float constantAttenuation, linearAttenuation, quadraticAttenuation;
	float spotCutoff, spotExponent;
	vec3 spotDirection;
};

uniform int lightNumber;
uniform light lights[10];

vec4 sceneAmbient = vec4(0.2, 0.2, 0.2, 1.0);

uniform vec3 viewPos;

void main()
{
	vec3 FragPos = texture(gPosition, TexCoords).rgb;
	vec3 Normal = texture(gNormal, TexCoords).rgb;
	vec3 Colour = texture(gColour, TexCoords).rgb;
	
	vec3 lighting = sceneAmbient.xyz;
	vec3 viewDir = normalize(viewPos - FragPos);
	
	for(int i = 0; i < lightNumber; ++i)
	{
		float distance = length(lights[i].position.xyz - FragPos);
	
		vec3 lightDir = normalize(lights[i].position.xyz - FragPos);
		vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Colour * lights[i].diffuse.xyz;
		
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
		vec3 specular = lights[i].specular.rgb * spec;
		float attenuation = 1.0 / 
			(lights[i].constantAttenuation + lights[i].linearAttenuation * distance +
			lights[i].quadraticAttenuation * pow(distance, 2));
			
		lighting += diffuse + specular;
	}
	
	FragColour = vec4(FragPos, 1.0);
}