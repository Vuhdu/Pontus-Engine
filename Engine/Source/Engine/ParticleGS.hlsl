#include "ParticleStructs.hlsli"

[maxvertexcount(3)]
void main(
	point VertexToGeometry anInput[1], 
	inout TriangleStream<GeometryToPixel> output
)
{
	const float2 offsets[4] =
	{
		{ -1.0f, 1.0f },
		{ 1.0f, 1.0f },
		{ -1.0f, -1.0f },
		{ 1.0f, -1.0f },
	};

	const float2 uvs[4] =
	{
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 0.0f, 1.0f },
		{ 1.0f, 1.0f },
	};

	VertexToGeometry inputVertex = anInput[0];
	for (unsigned int index = 0; index < 4; index++)
	{
		GeometryToPixel vertex;

		vertex.myPosition = inputVertex.myPosition;
		vertex.myPosition.xy += offsets[index] * inputVertex.mySize;
		vertex.myPosition = mul(FB_ToProjection, vertex.myPosition);
		vertex.myColor = inputVertex.myColor;
		vertex.myUV = uvs[index];
		vertex.myEmissiveStrength = inputVertex.myEmissiveStrength;

		output.Append(vertex);
	}
}