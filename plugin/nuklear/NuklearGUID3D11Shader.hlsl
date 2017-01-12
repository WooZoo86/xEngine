struct VS_INPUT
{
  float2 aPosition : POSITION;
  float4 aColor0 : COLOR;
  float2 aTexcoord0 : TEXCOORD;
};

struct VS_OUTPUT
{
  float4 Position : SV_POSITION;
  float4 Color : COLOR;
  float2 Texcoord : TEXCOORD;
};

#define PS_INPUT VS_OUTPUT

struct PS_OUTPUT
{
  float4 Color : SV_Target;
};

float4x4 uProjectionMatrix;

Texture2D uTexture;
SamplerState uSampler;

VS_OUTPUT VS(VS_INPUT input)
{
  VS_OUTPUT output;
  output.Position = mul(uProjectionMatrix, float4(input.aPosition.xy, 0.0, 1.0));
  output.Color = input.aColor0;
  output.Texcoord  = input.aTexcoord0;
  return output;
}

PS_OUTPUT PS(PS_INPUT input)
{
  PS_OUTPUT output;
  output.Color = input.Color * uTexture.Sample(uSampler, input.Texcoord);
  return output;
}