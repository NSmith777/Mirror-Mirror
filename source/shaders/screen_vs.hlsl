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
};

vs_out main(vs_in input)
{
    vs_out output;

    output.position = float4(input.position, 1.0f);
    output.texcoord = input.texcoord;

    return output;
}
