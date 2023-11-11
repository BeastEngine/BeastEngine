struct VS_Out
{
    float4 color : COLOR;
    float4 position : SV_POSITION;
};

VS_Out main(float2 position : POSITION, float4 color : COLOR)
{
    VS_Out output;
    output.color = color;
    output.position = float4(position, 0.0f, 1.0f);

    return output;
}

//struct VSIn
//{
//    uint vertexId : SV_VertexID;
//};

//struct VSOut
//{
//    float4 pos : SV_Position;
//    float4 color : color;
//};

//VSOut main(VSIn input)
//{
//    VSOut output;
    
//    if (input.vertexId == 0)
//        output.pos = float4(0.0, 0.5, 0.5, 1.0);
//    else if (input.vertexId == 2)
//        output.pos = float4(0.5, -0.5, 0.5, 1.0);
//    else if (input.vertexId == 1)
//        output.pos = float4(-0.5, -0.5, 0.5, 1.0);
    
//    output.color = clamp(output.pos, 0, 1);
    
//    return output;
//}

//float4 main(float2 position : POSITION) : SV_Position
//{
//    return float4(position.x, position.y, 0.0f, 1.0f);
//}