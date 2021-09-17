SamplerState defaultSampler		: register(s0);

Texture2D albedoTexture			: register(t1);
Texture2D normalTexture			: register(t2);
Texture2D materialTexture		: register(t3);

struct VertexToPixel
{
	float4 myPosition		:	SV_POSITION;
	float4 myWorldPosition	:	WORLDPOSITION;
	float myDepth			:	DEPTH;
	float4 myColor			:	COLOR0;
	float4 myColor1			:	COLOR1;
	float4 myColor2			:	COLOR2;
	float4 myColor3			:	COLOR3;
	float2 myUV				:	UV0;
	float2 myUV1			:	UV1;
	float2 myUV2			:	UV2;
	float2 myUV3			:	UV3;
	float4 myNormal			:	NORMAL;
	float4 myTangent		:	TANGENT;
	float4 myBinormal		:	BINORMAL;
};

cbuffer ObjectBuffer : register(b1)
{
	float4x4 toWorld;

	float2 myUVScale;
	float2 myTrashh;
}