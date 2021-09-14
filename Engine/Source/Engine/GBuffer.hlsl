#include "DeferredStructs.hlsli"
#include "PBRFunctions.hlsli"

GBufferOutput main(VertexOutput anInput)
{
	float2 scaledUV = anInput.myUV * OB_UVScale;
	float3 albedo = GammaToLinear(albedoTexture.Sample(defaultSampler, scaledUV).rgb);

	float3 normal = normalTexture.Sample(defaultSampler, scaledUV).wyz;
	float ambientOcclusion = normal.b;

	normal = 2.0f * normal - 1.0f;
	normal.z = sqrt(1 - saturate(normal.x * normal.x + normal.y * normal.y));
	normal = normalize(normal);

	float3x3 TBN = float3x3(
		normalize(anInput.myTangent.xyz),
		normalize(anInput.myBinormal.xyz),
		normalize(anInput.myNormal.xyz)
		);
	TBN = transpose(TBN);
	float3 pixelNormal = normalize(mul(TBN, normal));
	float3 material = materialTexture.Sample(defaultSampler, scaledUV).rgb;

	GBufferOutput output;

	output.myWorldPosition = anInput.myWorldPosition;
	output.myAlbedo = float4(albedo, 1.0f);
	output.myNormal = float4(pixelNormal, 1.0f);
	output.myVertexNormal = float4(anInput.myNormal.xyz, 1.0f);
	output.myMaterial = material;
	output.myAmbientOcclusion = anInput.myDepth;
	output.myDepth = ambientOcclusion;
	return output;
}