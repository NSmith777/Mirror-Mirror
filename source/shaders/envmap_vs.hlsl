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

inline float3 ObjSpaceViewDir(in float3 posWS)
{
    float3 objSpaceCameraPos = mul(_Object2World, float4(_WorldSpaceCameraPos, 1)).xyz;
    return objSpaceCameraPos - posWS.xyz;
}

vs_out main(vs_in input)
{
    vs_out output;

    output.position = mul(float4(input.position, 1.0f), MVP);

    // TexGen SphereMap
    float3 viewDir = normalize(ObjSpaceViewDir(input.position));
    float3 r = reflect(-viewDir, input.normal);
    r = mul((float3x3)MV, r);
    r.z += 1;
    float m = 2 * length(r);
    output.texcoord = r.xy / m + 0.5;

    output.color = float4(1.0f, 1.0f, 1.0f, 1.0f);

    return output;
}
