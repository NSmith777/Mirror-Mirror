cbuffer constants : register(b0)
{
    row_major float4x4 transform;
    row_major float4x4 projection;
    float3 lightvector;
}

struct vs_in
{
    float3 position : POS;
    float2 texcoord : TEX;
    float3 normal   : NOR;
};

struct vs_out
{
    float4 position : SV_POSITION;
    float2 texcoord : TEX;
    float4 color    : COL;
};

vs_out main(vs_in input)
{
    float light = clamp(dot(normalize(mul(float4(input.normal, 0.0f), transform).xyz), normalize(-lightvector)), 0.0f, 1.0f) * 0.8f + 0.2f;

    vs_out output;

    output.position = mul(float4(input.position, 1.0f), mul(transform, projection));
    output.texcoord = input.texcoord;
    output.color = float4(light, light, light, 1.0f);

    return output;
}
