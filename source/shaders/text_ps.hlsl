struct vs_out
{
    float4 position : SV_POSITION;
    float2 texcoord : TEX;
};

Texture2D    mytexture : register(t0);
SamplerState mysampler : register(s0);

float4 main(vs_out input) : SV_TARGET
{
    float4 sampled = float4(1.0f, 1.0f, 1.0f, mytexture.Sample(mysampler, input.texcoord).r);

    return float4(1.0f, 1.0f, 1.0f, 1.0f) * sampled;
}
