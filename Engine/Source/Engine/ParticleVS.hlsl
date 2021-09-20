#include "ParticleStructs.hlsli"

VertexToGeometry main(VertexInput anInput)
{
	VertexToGeometry result;

	float4 vertexObjectPos = anInput.myPosition;
	float4 vertexWorldPos = mul(OB_ToWorld, vertexObjectPos);
	float4 vertexViewPos = mul(FB_ToCamera, vertexWorldPos);

	result.myPosition = vertexViewPos;
	result.myColor = float4(anInput.myColor.rgb * anInput.myEmissiveStrength, anInput.myColor.a);
	result.mySize = anInput.mySize;
	result.myEmissiveStrength = anInput.myEmissiveStrength;

	return result;
}