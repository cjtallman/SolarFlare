
sampler2D Texture0 : TEXUNIT0;

float resource_percent;


float4 main( const float2 texcoord0 : TEXCOORD0, const float4 color : COLOR0 ) : COLOR0
{
  float4 full_color = tex2D(Texture0, texcoord0) * color;
  float luminosity = .3f; 
  float4 gray_color = float4(full_color.rgb * luminosity,full_color.a);
  
  float t = (1-texcoord0.y) - resource_percent > 0.0f;
  
  return lerp( full_color, gray_color, t );
}