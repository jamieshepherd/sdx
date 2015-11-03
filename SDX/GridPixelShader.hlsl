struct VS_OUTPUT
{
    float4 Position : SV_Position;
    //float4 TextureCoordinate : TEXCOORD;
    float4 Color : COLOR;
};

float4 main(VS_OUTPUT input) : SV_TARGET
{
    return input.Color;
    //return MeshTexture.Sample(MeshTextureSamplerState, input.TextureCoordinate);
}