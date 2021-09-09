#include "FullscreenStructs.hlsli"

PixelOutput main(VertexToPixel input)
{
	PixelOutput returnValue;
	returnValue.myColor.rgb = FullscreenTexture1.Sample(DefaultSampler, input.myUV).rbg;
	returnValue.myColor.a = 1.0f;
	return returnValue;
}