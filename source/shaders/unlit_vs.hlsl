cbuffer constants : register(b0)
{
    row_major float4x4 MVP;
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
    vs_out output;

    output.position = mul(float4(input.position, 1.0f), MVP);
    output.texcoord = input.texcoord;
    output.color = float4(1.0f, 1.0f, 1.0f, 1.0f);

    return output;
}
