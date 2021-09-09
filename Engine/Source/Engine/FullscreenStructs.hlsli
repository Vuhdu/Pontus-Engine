texture2D FullscreenTexture1 : register(t0);
texture2D FullscreenTexture2 : register(t1);

SamplerState DefaultSampler : register(s0);

static const float2 Resolution = float2(1280.0f, 720.0f) / 4.0f;
static const float GaussianKernel5[5] = { 0.06136f, 0.24477f, 0.38774f, 0.24477f, 0.06136f };

struct VertexInput
{
	unsigned int myIndex : SV_VertexID;
};

struct VertexToPixel
{
	float4 myPosition	: SV_POSITION;
	float2 myUV			: UV;
};

struct PixelOutput
{
	float4 myColor		: SV_TARGET;
};