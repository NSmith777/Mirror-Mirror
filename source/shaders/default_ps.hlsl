struct vs_out
{
    float4 position : SV_POSITION;
    float2 texcoord : TEX;
    float4 color    : COL;
};

Texture2D    mytexture : register(t0);
SamplerState mysampler : register(s0);

float4 main(vs_out input) : SV_TARGET
{
    return mytexture.Sample(mysampler, input.texcoord) * input.color;
}
