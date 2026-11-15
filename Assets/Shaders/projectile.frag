
sampler2D texture0 : TEXUNIT0;

float4 model_color;
float4 glow_color;
float glow_amount;

float4 main( const float2 texcoord0 : TEXCOORD0 ) : COLOR0
{  
  float4 Kt = tex2D(texture0, texcoord0);
  float4 color = Kt.a * .2f * Kt + glow_amount * glow_color * Kt + model_color * Kt;
  return 2 * float4(color.rgb, Kt.a*2);
}