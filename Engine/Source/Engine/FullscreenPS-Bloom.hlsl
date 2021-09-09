#include "FullscreenStructs.hlsli"

PixelOutput main(VertexToPixel input)
{
	PixelOutput result;

	float3 resource1 = FullscreenTexture1.Sample(DefaultSampler, input.myUV).rbg;
	float3 resource2 = FullscreenTexture1.Sample(DefaultSampler, input.myUV).rbg;

	{
		float luminance = dot(resource1, float3(0.2126f, 0.7152f, 0.0722f));
		float3 scaledResource = resource2 * (1.0f - luminance);
		result.myColor.rgb = resource1 + scaledResource;
	}

	result.myColor.a = 1.0f;

	return result;
}