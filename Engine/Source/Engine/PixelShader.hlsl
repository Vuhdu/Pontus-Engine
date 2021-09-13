#include "ShaderStructs.hlsli"
#include "PBRFunctions.hlsli"

float3 PixelShader_Albedo(VertexOutput aInput)
{
	float3 albedo = albedoTexture.Sample(defaultSampler, aInput.myUV0.xy).rgb;
	return GammaToLinear(albedo);
}

float3 PixelShader_Normal(VertexOutput aInput)
{
	float3 normal = float3(normalTexture.Sample(defaultSampler, aInput.myUV0.xy).wy, 0);

	normal = 2.0 * normal - 1.0;
	normal.z = sqrt(1.0 - saturate(normal.x * normal.x + normal.y * normal.y));

	float3x3 TBN = float3x3(
		normalize(aInput.myTangent.xyz),
		normalize(aInput.myBinormal.xyz),
		normalize(aInput.myNormal.xyz)
		);

	normal = mul(normal.xyz, TBN);
	normal = normalize(normal);

	return normal.xyz;
}

float3 PixelShader_Material(VertexOutput aInput)
{
	return materialTexture.Sample(defaultSampler, aInput.myUV0.xy).rgb;
}

float PixelShader_Metalness(VertexOutput aInput)
{
	return PixelShader_Material(aInput).r;
}

float PixelShader_PerceptualRoughness(VertexOutput aInput)
{
	return PixelShader_Material(aInput).g;
}

float PixelShader_Emissive(VertexOutput aInput)
{
	return PixelShader_Material(aInput).b;
}

float PixelShader_AmbientOcclusion(VertexOutput aInput)
{
	return normalTexture.Sample(defaultSampler, aInput.myUV0.xy).b;
}

PixelOutput main(VertexOutput aInput)
{
	float3 toEye = normalize(cameraPosition.xyz - aInput.myWorldPosition.xyz);
	float3 albedo = PixelShader_Albedo(aInput);
	float3 normal = PixelShader_Normal(aInput);
	float ambientOcclusion = PixelShader_AmbientOcclusion(aInput);
	float metalness = PixelShader_Metalness(aInput);
	float perceptualRoughness = PixelShader_PerceptualRoughness(aInput);
	float emissiveData = PixelShader_Emissive(aInput);

	float3 specularColor = lerp((float3)0.04, albedo, metalness);
	float3 diffuseColor = lerp((float3)0.0, albedo, 1.0 - metalness);

	float3 ambience = EvaluateAmbience(
		environmentTexture,
		normal,
		aInput.myNormal.xyz,
		toEye,
		perceptualRoughness,
		metalness,
		albedo,
		ambientOcclusion,
		diffuseColor,
		specularColor);

	float3 directionalLight = EvaluateDirectionalLight(
		diffuseColor,
		specularColor,
		normal,
		perceptualRoughness,
		directionalLightColor.xyz,
		toDirectionalLight.xyz,
		toEye.xyz);

	float3 pointLights = 0;
	for (unsigned int i = 0; i < OB_NumPointLights; i++)
	{
		pointLights += EvaluatePointLight(
			diffuseColor, specularColor, normal, perceptualRoughness,
			myPointLights[i].Color.rgb, myPointLights[i].Color.w, myPointLights[i].Range, 
			myPointLights[i].Position.xyz,
			toEye.xyz, aInput.myWorldPosition.xyz
		);
	}

	float3 spotLights = 0;
	for (unsigned int s = 0; s < OB_NumSpotLights; s++)
	{
		spotLights += EvaluateSpotLight(
			diffuseColor, specularColor, normal, perceptualRoughness,
			mySpotLights[s].Color.rgb, mySpotLights[s].Color.w, mySpotLights[s].Range,
			mySpotLights[s].Position.xyz, mySpotLights[s].Direction.xyz,
			mySpotLights[s].OuterAngle, mySpotLights[s].InnerAngle, 
			toEye.xyz, aInput.myWorldPosition.xyz
		);
	}

	float3 dullAmbience = ambience * 0.2f;
	float3 emissive = albedo * emissiveData;
	float3 radiance = dullAmbience + directionalLight + pointLights + spotLights + emissive;

	PixelOutput pixelOutput;
	pixelOutput.myColor.rgb = LinearToGamma(radiance);
	pixelOutput.myColor.a = 1.0f;

	return pixelOutput;
}