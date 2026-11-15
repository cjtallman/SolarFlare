sampler2D Texture0 : TEXUNIT0;
sampler2D Texture1 : TEXUNIT1;

float4 main( float2 texcoord0 : TEXCOORD0 ) : COLOR0
{
  float4 color = tex2D( Texture0, texcoord0 );
  color += tex2D( Texture1, texcoord0 );
  return color;
} 


