sampler2D Texture0 : TEXUNIT0;

float4 main( float2 texcoord0 : TEXCOORD0 ) : COLOR0
{
  float threshold = 1.0f;
  float4 color = tex2D( Texture0, texcoord0 );
  float lum = dot( float4(0.30f, 0.59f, 0.11f, 0.05f), color );

	if (lum <= threshold)
		color = float4(0.0, 0.0, 0.0, 1.0);

  return color;
}