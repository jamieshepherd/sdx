////**************************************************************************//
//// Pixel shader input structure.	Just a position and a colour.			//
////																			//
//// NOTE: Pos has a different samentic to the structure above; get it wrong	//
//// and nothing works.  That's because the pixel shader is in a different	//
//// stage in the rendering pipeline.											//
////**************************************************************************//
//struct PS_INPUT
//{
//    float4 Pos : SV_POSITION;
//    float4 Color : COLOR;
//};

////**************************************************************************//
//// The pixel shader. There isn't much obviously going on here, we just		//
//// extract the colour from the imput and return it.							//
////																			//
//// But there is clever stuff going on that's hidden from us.  Each vertex of//
//// each cube is defined as a seperate colour.  The shader automatically     //
//// interpolates (blends if you wish) the colour between vertices.			//
////**************************************************************************//
//float4 PShader(PS_INPUT input) : SV_Target
//{
//    return input.Color;
//}

struct VS_OUTPUT
{
    float4 Position : SV_Position;
    float4 Color : COLOR;
};

float4 main(VS_OUTPUT IN) : SV_TARGET
{
    return IN.Color;
}