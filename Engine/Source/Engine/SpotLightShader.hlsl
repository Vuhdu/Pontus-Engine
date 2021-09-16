#include "DeferredStructs.hlsli"
#include "PBRFunctions.hlsli"

cbuffer SpotLightBuffer : register(b1)
{
	float4 myPosition;
	float4 myDirection;
	float4 myColorAndIntensity;
	float myRange;
	float myInnerAngle;
	float myOuterAngle;
	float trash;
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

	float ao = ambientOcclusionTexture.Sample(defaultSampler, input.myUV).r;
	float depth = depthTexture.Sample(defaultSampler, input.myUV).r;

	float3 toEye = normalize(FB_CameraPosition.xyz - worldPosition.xyz);

	float3 specularColor = lerp((float3)0.04, albedo, metalness);
	float3 diffuseColor = lerp((float3)0.00, albedo, 1 - metalness);

	float3 spotLights = EvaluateSpotLight(
		diffuseColor,
		specularColor,
		normal,
		roughness,
		myColorAndIntensity.rgb,
		myColorAndIntensity.w,
		myRange,
		myPosition.xyz,
		myDirection.xyz,
		myOuterAngle,
		myInnerAngle,
		toEye.xyz,
		worldPosition.xyz);

	float3 radiance = spotLights;

	output.myColor.rgb = LinearToGamma(radiance);
	output.myColor.a = 1.0f;

	return output;
}