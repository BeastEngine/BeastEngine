Texture2D basicTexture;
SamplerState basicSampler;

float4 main(float4 color : COLOR, float2 uv : TEXCOORDS) : SV_TARGET {
  float4 texelColor = basicTexture.Sample(basicSampler, uv);

  return texelColor * color;
  return color;
}
