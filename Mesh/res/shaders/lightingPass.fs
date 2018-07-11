#version 330 core

layout (location = 0) out vec4 FragColour;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColour;
uniform sampler2D gDepth;

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

vec4 sceneAmbient = vec4(0.1, 0.1, 0.1, 1.0);

uniform vec3 viewPos;

//What data to draw
uniform int drawType;

const int LIGHTING = 0;
const int POSITION = 1;
const int NORMALS = 2;
const int COLOUR = 3;
const int DEPTH = 4;

void main()
{
	vec3 FragPos = texture(gPosition, TexCoords).rgb;
	vec3 Normal = texture(gNormal, TexCoords).rgb;
	vec3 Colour = texture(gColour, TexCoords).rgb;
	
	vec3 data;
	
	if(drawType == LIGHTING)
	{
		vec3 totalLight = sceneAmbient.xyz;
		vec3 viewDir = normalize(viewPos - FragPos);
		
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
				vec3 vertToLight = vec3(lights[i].position.xyz - FragPos);
				float dist = length(vertToLight);
				lightDir = normalize(vertToLight);
				attenuation = 1.0 / 
					(lights[i].constantAttenuation + lights[i].linearAttenuation * dist +
					lights[i].quadraticAttenuation * pow(dist, 2.0));
			}
			
			vec3 diffuse = attenuation * max(dot(Normal, lightDir), 0.0) * Colour * lights[i].diffuse.xyz;
			
			vec3 halfwayDir = normalize(lightDir + viewDir);
			float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
			vec3 specular = lights[i].specular.rgb * spec * attenuation;
				
			totalLight += diffuse + specular;
		}
		
		data = totalLight;
	}
	else if (drawType == POSITION)
	{
		data = FragPos;
	}
	else if (drawType == NORMALS)
	{
		data = Normal;
	}
	else if (drawType == COLOUR)
	{
		data = Colour;
	}
	else if (drawType == DEPTH)
	{
		data = texture(gDepth, TexCoords).rgb;
	}
	
	FragColour = vec4(data, 1.0);
}