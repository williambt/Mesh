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
uniform int hasNormalMap;
uniform sampler2D uNormalMap;
uniform mat3 inv_transp;

uniform material mtl;

in VS_OUT {
	vec3 normal;
	vec2 uv;
	vec4 worldPos;
	mat3 TBN;
} fs_in;

in mat4 viewMatrix;

void main()
{
	vec3 normal = fs_in.normal;
	if(hasNormalMap == 1)
	{
		normal = texture(uNormalMap, fs_in.uv).rgb;
		normal = normalize(normal * 2.0 - 1.0);
		normal = normalize(fs_in.TBN * normal);
	}
	
	vec3 normalDir = normal;
	vec3 viewDir = normalize(vec3(inverse(viewMatrix) * vec4(0, 0, 0, 1) - fs_in.worldPos));
	vec3 lightDir;
	float attenuation;
	
	vec3 totalLight = vec3(sceneAmbient) * vec3(mtl.ambient);
	
	vec4 texel = texture(uTexture, fs_in.uv);
	
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
			vec3 vertToLight = vec3(lights[i].position.xyz - fs_in.worldPos.xyz);
			float dist = length(vertToLight);
			lightDir = normalize(vertToLight);
			attenuation = 1.0 / 
				(lights[i].constantAttenuation + lights[i].linearAttenuation * dist +
				lights[i].quadraticAttenuation * pow(dist, 2.0));
		}
		
		vec3 diffuse = attenuation * max(dot(normalDir, lightDir), 0.0) * texel.rgb * mtl.diffuse * lights[i].diffuse.xyz;
		
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(normalDir, halfwayDir), 0.0), mtl.shine);
		vec3 specular = lights[i].specular.rgb * spec * attenuation * mtl.specular;
			
		totalLight += diffuse + specular;
	}
	
	frag_colour = vec4(totalLight, 0.0) * texel;
}