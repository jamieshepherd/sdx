cbuffer cbPerObject
{
    float4x4 WVP;
    float4 g_tailAngle;
}

struct VS_INPUT
{
    float4 Position : POSITION;
    float4 TextureCoordinate : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 Position : SV_Position;
    float4 TextureCoordinate : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;

    float tailAngle = g_tailAngle.x;
    float tailBase = 560;
    float tailHeight = 200;

    // Check the height so we don't clip his back heels
    if (input.Position.z > tailBase && input.Position.y > tailHeight)
    {
        float distanceFromBase = tailBase - input.Position.z;		
        input.Position.x += tailAngle * distanceFromBase;
    }

    output.Position = mul(input.Position, WVP);
    output.TextureCoordinate = input.TextureCoordinate;

    return output;
}