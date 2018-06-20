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
	float spotCutoff, spotExponent;
	vec3 spotDirection;
};

vec4 sceneAmbient = vec4(0.2, 0.2, 0.2, 1.0);

uniform int lightNumber;
uniform light lights[10];

uniform sampler2D uTexture;
uniform mat3 inv_transp;
uniform material mtl;

in vec3 normal;
in vec2 uv;
in vec4 worldPos;
in mat4 viewMatrix;

void main()
{
	vec3 normalDir = normalize(inv_transp * normal);
	vec3 viewDir = normalize(vec3(inverse(viewMatrix) * vec4(0, 0, 0, 1) - worldPos));
	vec3 lightDir;
	float attenuation;
	
	vec3 totalLight = vec3(sceneAmbient) * vec3(mtl.ambient);
	
	vec4 texel = texture(uTexture, uv);
	
	for(int i = 0; i < lightNumber; i++)
	{
		if(lights[i].position.w == 0.0) //Directional
		{
			attenuation = 1.0;
			lightDir = normalize(vec3(lights[i].position));
		}
		else //Point or Spot
		{
			vec3 vertToLight = vec3(lights[i].position - worldPos);
			float distance = length(vertToLight);
			lightDir = normalize(vertToLight);
			attenuation = 1.0 / (lights[i].constantAttenuation 
				+ lights[i].linearAttenuation * distance
				+ lights[i].quadraticAttenuation * distance * distance);
			
			if(lights[i].spotCutoff
		}
		
		vec3 diffReflection = attenuation * vec3(lights[i].diffuse) * (vec3(mtl.diffuse)) * max(0.0, dot(normalDir, lightDir));
		vec3 specReflection;
		if(dot(normalDir, lightDir) < 0.0)
		{
			specReflection = vec3(0.0, 0.0, 0.0);
		}
		else
		{
			specReflection = attenuation * vec3(lights[i].specular) * vec3(mtl.specular) *
				pow(max(0.0, dot(reflect(-lightDir, normalDir), viewDir)), mtl.shine);
		}
		
		totalLight += diffReflection + specReflection;
	}
	
	
	frag_colour = vec4(totalLight, 1.0) * texel;
}