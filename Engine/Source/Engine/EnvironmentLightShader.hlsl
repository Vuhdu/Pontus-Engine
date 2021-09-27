#include "DeferredStructs.hlsli"
#include "PBRFunctions.hlsli"

cbuffer EnvironmentLightBuffer : register(b1)
{
    float4x4 lightView;
    float4x4 lightProjection;
	float4 toDirectionalLight;
	float4 directionalLightColor;
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

	float4 worldPosition = positionTexture.Sample(defaultSampler, input.myUV).rgba;
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

    float4 worldToLightView = mul(lightView, worldPosition);
    float4 lightViewToProj = mul(lightProjection, worldToLightView);
	
    float2 projectedTexCoord;
    projectedTexCoord.x = lightViewToProj.x / lightViewToProj.w / 2.0f + 0.5f;
    projectedTexCoord.y = lightViewToProj.y / lightViewToProj.w / 2.0f + 0.5f;
	
	if (saturate(projectedTexCoord.x) == projectedTexCoord.x && saturate(projectedTexCoord.y) == projectedTexCoord.y)
    {
        const float shadowBias = 0.0005f;
		
        float shadow = 0.0f;
		
        float viewDepth = (lightViewToProj.z / lightViewToProj.w) - shadowBias;
		
        float sampleDepth = shadowTexture.Sample(defaultSampler, projectedTexCoord).r;
		
		if (sampleDepth < viewDepth)
        {
            directionalLight *= shadow;
        }
    }
	
	float3 emissive = albedo * emissiveMask;
	float3 radiance = ambience + directionalLight + emissive;

	output.myColor.rgb = LinearToGamma(radiance);
	output.myColor.a = 1.0f;

	return output;
}