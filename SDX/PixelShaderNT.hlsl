struct VS_OUTPUT
{
    float4 Position : SV_Position;
    float4 TextureCoordinate : TEXCOORD;
    //float4 Color : COLOR;
};

float4 mainNT() : SV_TARGET
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}