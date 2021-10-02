#include "DeferredStructs.hlsli"
#include "PBRFunctions.hlsli"

cbuffer PointLightBuffer : register(b1)
{
    float4x4 lightView[6];
    float4x4 lightProjection;
	float4 myPosition;
	float4 myColorAndIntensity;
	float myRange;
	float3 garbage;
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

	float4 worldPosition = positionTexture.Sample(defaultSampler, input.myUV);
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

	float3 pointLight = EvaluatePointLight(
		diffuseColor,
		specularColor,
		normal,
		roughness,
		myColorAndIntensity.rgb,
		myColorAndIntensity.w,
		myRange,
		myPosition.xyz,
		toEye.xyz,
		worldPosition.xyz);

    for (int i = 0; i < 6; i++)
    {
        float4 worldToLightView = mul(lightView[i], worldPosition);
        float4 lightViewToLightProj = mul(lightProjection, worldToLightView);

        float3 projectedTexCoord;
        projectedTexCoord.x = lightViewToLightProj.x / lightViewToLightProj.w / 2.f + 0.5f;
        projectedTexCoord.y = -lightViewToLightProj.y / lightViewToLightProj.w / 2.f + 0.5f;
        projectedTexCoord.z = lightViewToLightProj.z / lightViewToLightProj.w / 2.f + 0.5f;
	
        if (saturate(projectedTexCoord.x) == projectedTexCoord.x &&
			saturate(projectedTexCoord.y) == projectedTexCoord.y &&
			saturate(projectedTexCoord.z) == projectedTexCoord.z)
        {
            const float shadowBias = 0.0005f;
		
            float shadow = 0.f;
		
            float viewDepth = (lightViewToLightProj.z / lightViewToLightProj.w) - shadowBias;

            float sampleDepth = ShadowTextures[i].Sample(pointSampler, (float2) projectedTexCoord).r;
		
            if (sampleDepth < viewDepth)
            {
                pointLight *= shadow;
            }
        }
    }
	
	float3 radiance = pointLight;

	output.myColor.rgb = LinearToGamma(radiance);
	output.myColor.a = 1.0f;

	return output;
}