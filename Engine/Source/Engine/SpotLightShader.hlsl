#include "DeferredStructs.hlsli"
#include "PBRFunctions.hlsli"

PixelOutput main(VertexOutput anInput)
{
	float4 albedoData = albedoTexture.Sample(defaultSampler, anInput.myUV);
	if (albedoData.a == 0)
		discard;

	float3 albedo = albedoData.rgb;

	PixelOutput output;

	float3 worldPosition = positionTexture.Sample(defaultSampler, anInput.myUV).rgb;
	float3 normal = normalTexture.Sample(defaultSampler, anInput.myUV).xyz;
	float3 vertexNormal = vertexNormalTexture.Sample(defaultSampler, anInput.myUV).xyz;
	float4 material = materialTexture.Sample(defaultSampler, anInput.myUV);

	float metalness = material.r;
	float roughness = material.g;
	float emissiveMask = material.b;

	float ao = ambientOcclusionTexture.Sample(defaultSampler, anInput.myUV).r;
	float depth = depthTexture.Sample(defaultSampler, anInput.myUV).r;

	float3 toEye = normalize(cameraPosition.xyz - worldPosition.xyz);

	float3 specularColor = lerp((float3)0.04, albedo, metalness);
	float3 diffuseColor = lerp((float3)0.00, albedo, 1 - metalness);

	float3 ambience = EvaluateAmbience(
		environmentTexture,
		normal,
		vertexNormal,
		toEye,
		roughness,
		material,
		albedo,
		ao,
		diffuseColor,
		specularColor
	);

	float3 spotLights = 0;
	for (unsigned int s = 0; s < numSpotLights; s++)
	{
		spotLights += EvaluateSpotLight(
			diffuseColor, specularColor, normal, roughness,
			mySpotLights[s].Color.rgb, mySpotLights[s].Color.w, mySpotLights[s].Range,
			mySpotLights[s].Position.xyz, mySpotLights[s].Direction.xyz,
			mySpotLights[s].OuterAngle, mySpotLights[s].InnerAngle,
			toEye.xyz, anInput.myWorldPosition.xyz
		);
	}

	float3 emissive = albedo * emissiveMask;
	float3 radiance = ambience + spotLights + emissive;

	output.myColor.rgb = radiance;
	output.myColor.a = 1.0f;
	return output;
}