struct VS_Out {
  float4 color : COLOR;
  float2 uv : TEXCOORDS;
  float4 position : SV_POSITION;
};

cbuffer ViewProjection { matrix vpMatrix; };

VS_Out main(float2 position
            : POSITION, float4 color
            : COLOR, float2 uv
            : TEXCOORDS) {
  VS_Out output;
  output.color = color;
  output.position = mul(vpMatrix, float4(position, 0.0f, 1.0f));
  output.position.w = 1.0f;
  // output.position = float4(position, 0.0f, 1.0f);

  output.uv = uv;
  // output.uv = float2(0.0f, 0.0f);

  return output;
}
