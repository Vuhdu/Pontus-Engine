#include "DeferredStructs.hlsli"
#include "PBRFunctions.hlsli"

cbuffer EnvironmentLightBuffer : register(b1)
{
	float4 toDirectionalLight;
	float4 directionalLightColor;

	int myEnvironmentLightMipCount; // No longer used (fråga filip ?)
	int myTrash[3];
}

PixelOutput main(VertexToPixel input)
{
	float4 albedoData = albedoTexture.Sample(defaultSampler, input.myUV);

	if (albedoData.a == 0)
	{
		discard;
	}

	float3 albedo = albedoData.rgb;

	PixelOutput output;

	float3 worldPosition = positionTexture.Sample(defaultSampler, input.myUV).rgb;
	float3 normal = normalTexture.Sample(defaultSampler, input.myUV).xyz;
	float3 vertexNormal = vertexNormalTexture.Sample(defaultSampler, input.myUV).xyz;
	float4 material = materialTexture.Sample(defaultSampler, input.myUV);

	float metalness = material.r;
	float roughness = material.g;
	float emissiveMask = material.b;

	float ao = ambientOcclusionTexture.Sample(defaultSampler, input.myUV).r;
	float depth = depthTexture.Sample(defaultSampler, input.myUV).r;

	float3 toEye = normalize(FB_CameraPosition.xyz - worldPosition.xyz);

	float3 specularColor = lerp((float3)0.04, albedo, metalness);
	float3 diffuseColor = lerp((float3)0.00, albedo, 1 - metalness);

	float3 ambience = EvaluateAmbience(
		environmentTexture,
		normal,
		vertexNormal,
		toEye,
		roughness,
		metalness,
		albedo,
		ao,
		diffuseColor,
		specularColor
	);

	float3 directionalLight = EvaluateDirectionalLight(
		diffuseColor,
		specularColor,
		normal,
		roughness,
		directionalLightColor.rgb,
		toDirectionalLight.xyz,
		toEye.xyz
	);

	float3 emissive = albedo * emissiveMask;
	float3 radiance = ambience + directionalLight + emissive;

	output.myColor.rgb = LinearToGamma(radiance);
	output.myColor.a = 1.0f;

	return output;
}