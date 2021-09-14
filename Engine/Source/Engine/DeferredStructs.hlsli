TextureCube environmentTexture      : register(t0);

Texture2D albedoTexture				: register(t1);
Texture2D normalTexture				: register(t2);
Texture2D materialTexture			: register(t3);
Texture2D positionTexture			: register(t4);
Texture2D vertexNormalTexture		: register(t5);
Texture2D ambientOcclusionTexture	: register(t6);
Texture2D depthTexture				: register(t7);

SamplerState defaultSampler			: register(s0);

cbuffer EnvironmentLightBuffer
{
    float4 toDirectionalLight;
    float4 directionalLightColor;

    int environmentLightMipCount;
    int trash[3];
};

cbuffer FrameBuffer : register(b0)
{
    float4x4 toCamera;
    float4x4 toProjection;

    float4 cameraPosition;
}

cbuffer ObjectBuffer : register(b1)
{
    float4x4 toWorld;
    float2 OB_UVScale;

    unsigned int numPointLights;
    unsigned int numSpotLights;

    struct PointLightData
    {
        float4 Position;
        float4 Color;
        float Range;
        float3 Garbage;
    } myPointLights[8];

    struct SpotLightData
    {
        float4 Position;
        float4 Direction;
        float4 Color;
        float Range;
        float InnerAngle;
        float OuterAngle;
        float Trash;
    } mySpotLights[8];
}

struct VertexOutput
{
    float4 myPosition       : SV_POSITION;
    float4 myWorldPosition  : WORLDPOSITION;
    float myDepth           : DEPTH;
    float4 myVxColor        : COLOR0;
    float4 myVxColor2       : COLOR1;
    float4 myVxColor3       : COLOR2;
    float4 myVxColor4       : COLOR3;
    float2 myUV             : UV0;
    float2 myUV2            : UV1;
    float2 myUV3            : UV2;
    float2 myUV4            : UV3;
    float3 myNormal         : NORMAL;
    float3 myTangent        : TANGENT;
    float3 myBinormal       : BINORMAL;
};

struct PixelOutput
{
    float4 myColor          : SV_TARGET;
};