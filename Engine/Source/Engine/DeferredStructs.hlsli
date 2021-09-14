TextureCube environmentTexture      : register(t0);

Texture2D albedoTexture				: register(t1);
Texture2D normalTexture				: register(t2);
Texture2D materialTexture			: register(t3);
Texture2D positionTexture			: register(t4);
Texture2D vertexNormalTexture		: register(t5);
Texture2D ambientOcclusionTexture	: register(t6);
Texture2D depthTexture				: register(t7);

SamplerState defaultSampler			: register(s0);

struct GBufferOutput
{
    float4 myAlbedo					: SV_TARGET1;
    float4 myNormal					: SV_TARGET2;
    float3 myMaterial				: SV_TARGET3;
    float4 myVertexNormal			: SV_TARGET4;
    float4 myWorldPosition			: SV_TARGET5;
    float myAmbientOcclusion        : SV_TARGET6;
    float myDepth                   : SV_TARGET7;
};

cbuffer EnvironmentLightBuffer : register(b0)
{
    float4 toDirectionalLight;
    float4 directionalLightColor;

    int environmentLightMipCount;
    int trash[3];
};

cbuffer SpotLightBuffer : register(b1)
{
    float4 slPosition;
    float4 slDirection;
    float4 slColor;
    float slRange;
    float slInnerAngle;
    float slOuterAngle;
    float slTrash;
};

cbuffer PointLightBuffer : register(b2)
{
    float4 plPosition;
    float4 plColor;
    float plRange;
    float3 plGarbage;
};

cbuffer FrameBuffer : register(b3)
{
    float4x4 toCamera;
    float4x4 toProjection;

    float4 cameraPosition;
}

cbuffer ObjectBuffer : register(b4)
{
    float4x4 toWorld;
    float2 OB_UVScale;
    int padding[2];
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