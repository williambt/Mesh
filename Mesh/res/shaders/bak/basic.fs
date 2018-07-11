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
	
	vec3 totalLight = vec3(sceneAmbient) * vec3(mtl.ambient);
	
	vec4 texel = texture(uTexture, uv);
	
	for(int i = 0; i < lightNumber; ++i)
	{
		vec3 lightDir;
		float attenuation;
		if(lights[i].position.w == 0.0) //Directional
		{
			attenuation = 0.0;
			lightDir = normalize(vec3(lights[i].position));
		}
		else
		{
			vec3 vertToLight = vec3(lights[i].position.xyz - worldPos.xyz);
			float dist = length(vertToLight);
			lightDir = normalize(vertToLight);
			attenuation = 1.0 / 
				(lights[i].constantAttenuation + lights[i].linearAttenuation * dist +
				lights[i].quadraticAttenuation * pow(dist, 2.0));
		}
		
		vec3 diffuse = attenuation * max(dot(normalDir, lightDir), 0.0) * texel.rgb * lights[i].diffuse.xyz;
		
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(normalDir, halfwayDir), 0.0), 16.0);
		vec3 specular = lights[i].specular.rgb * spec * attenuation;
			
		totalLight += diffuse + specular;
	}	
	
	frag_colour = vec4(normal, 1.0) * texel;
}