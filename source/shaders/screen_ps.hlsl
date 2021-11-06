struct vs_out
{
    float4 position : SV_POSITION;
    float2 texcoord : TEX;
};

Texture2DMS<float4> myTexture;

float4 main(vs_out input) : SV_TARGET
{
    float4 returncol = float4(0, 0, 0, 0);

    for (uint i = 0; i < 4; i++)
    {
        returncol += myTexture.Load(int2(input.position.x, input.position.y), i);
    }
    return returncol / 4;
}
