cbuffer cbPerObject
{
    float4x4 WVP;
}

struct VS_INPUT
{
    float4 Position : POSITION;
    //float4 TextureCoordinate : TEXCOORD;
    float4 Color : COLOR;
};

struct VS_OUTPUT
{
    float4 Position : SV_Position;
    //float4 TextureCoordinate : TEXCOORD;
    float4 Color : COLOR;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

    output.Position = mul(input.Position, WVP);
    output.Color = input.Color;
    //output.TextureCoordinate = input.TextureCoordinate;

    return output;
}