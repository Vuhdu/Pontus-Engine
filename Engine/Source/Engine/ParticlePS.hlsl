#include "ParticleStructs.hlsli"

PixelOutput main(GeometryToPixel anInput)
{
	PixelOutput result;

	float4 textureColor = ParticleTexture.Sample(DefaultSampler, anInput.myUV);
	result.myColor.rgba = textureColor.rgba * anInput.myColor.rgba;

	return result;
}