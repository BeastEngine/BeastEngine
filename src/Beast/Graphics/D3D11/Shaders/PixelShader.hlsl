float4 main(float4 color : COLOR) : SV_TARGET
{
    //float4 texelColor = simpTexture.Sample(samplerState, uv);
    //float4 texelColor = 1.0f;

    //return texelColor * color;
    
    return color;
}

// NoBuffer.fx
// Copyright (c) 2004 Microsoft Corporation. All rights reserved.
//

//struct PSIn
//{
//    float4 pos : SV_Position;
//    linear float4 color : color;
//};

//struct PSOut
//{
//    float4 color : SV_Target;
//};


//PSOut main(PSIn input)
//{
//    PSOut output;
    
//    output.color = input.color;
    
//    return output;
//}

//float4 main() : SV_Target
//{
//    return float4(1.0f, 0.0f, 1.0f, 1.0f);
//}