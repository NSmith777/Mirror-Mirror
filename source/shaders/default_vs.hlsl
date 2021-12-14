cbuffer constants : register(b0)
{
    row_major float4x4 MVP;
    float4x4 MV;
    float4x4 M;
    float4x4 _Object2World;
    float3 _WorldSpaceCameraPos;
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

    // TODO: Configurable light component
    float3 _WorldSpaceLightPos0 = normalize(float3(0.4f, 1.0f, -0.6f));
    float3 normalDirection = normalize(mul(float4(input.normal, 0.0), _Object2World).xyz);

    float3 diffuseReflection = max(0.4, dot(normalDirection, _WorldSpaceLightPos0)) * 1.5f;

    output.position = mul(float4(input.position, 1.0f), MVP);
    output.texcoord = input.texcoord;
    output.color = float4(diffuseReflection, 1.0f);

    return output;
}
