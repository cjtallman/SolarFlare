sampler2D Texture0 : TEXUNIT0;

float brightness;

float4 main( const float2 texcoord0 : TEXCOORD0, const float4 color : COLOR0 ) : COLOR0
{
  float4 output = brightness * color * tex2D(Texture0, texcoord0);
  output.a = 1.16;
  return output;
}