SamplerState DefaultSampler : register(s0);

cbuffer FrameBufferData : register(b0)
{
    float4x4 FB_ToCamera;
    float4x4 FB_ToProjection;
};

cbuffer ObjectBufferData: register(b1)
{
    float4x4 OB_ToWorld;
};
