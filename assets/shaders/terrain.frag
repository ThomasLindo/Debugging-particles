#include "version.glsl"

#include "math/core.glsl"

uniform vec3 uSceneAmbient = vec3(0.6, 0.6, 0.6);
uniform vec3 uLightPosition = vec3(3.0, 0.0, 0.0);
uniform vec3 uLightColor = vec3(1.0, 0.9, 0.8);
uniform float uLightSpecularExponent = 16.0;

layout(binding = 0) uniform sampler2D uTexAlbedo;
layout(binding = 1) uniform sampler2D uTexEmissive;
layout(binding = 2) uniform sampler2D uTexSpecular;
layout(binding = 10) uniform sampler2D uTexNoise;
layout(binding = 11) uniform sampler3D uTexTerrain;

#include "vData.glsl"
in vData o;

out vec4 outColor;

void main()
{
	vec2 texOffset = o.texcoord;

	vec4 albedoColor = texture(uTexTerrain, vec3(o.worldPos.xz * 0.1, imix(-1, 4, o.worldPos.y)));
	outColor.rgb = albedoColor.rgb * uSceneAmbient; 
	outColor.a = albedoColor.a;

	// Fix length after rasterizer interpolates
	vec3 normal = normalize(o.norm);

	vec3 lightVec = uLightPosition - o.pos;
	float dist = length(lightVec);
	vec3 lightDir = lightVec / dist;

	float NdotL = dot(normal, lightDir);

	// If the normal is facing the light
	if (NdotL > 0.0)
	{
		// Normalized vector pointing towards the camera
		vec3 eye = normalize(-o.pos);
		
		// Calculate attenuation (falloff)
		// Add a small number to avoid divide by zero.
		float attenuation = 1.0 / (1.0 + dist * dist * 0.0001);

		// Calculate the diffuse contribution
		outColor.rgb += albedoColor.rgb * uLightColor * NdotL * attenuation;
		
		vec3 reflection = reflect(-lightDir, normal);
		
		float specularStrength = dot(reflection, eye);
		specularStrength = max(specularStrength, 0.0f); // don't let it fall before zero

		// Calculate the specular contribution
		outColor.rgb += texture(uTexSpecular, texOffset).rgb * uLightColor * pow(specularStrength, uLightSpecularExponent) * attenuation;
	}

	outColor.rgb += texture(uTexEmissive, texOffset).rgb;
}
