struct VS_OUTPUT
{
    float4 Position : SV_Position;
    float4 Color : COLOR0;
};

float4 main(VS_OUTPUT input) : SV_TARGET
{
    return input.Color;
}