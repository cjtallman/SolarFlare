
sampler2D Texture0 : TEXUNIT0;

float2 PixelSize;

const float coeff[3] = 
{ 
  1.0f/4.0f, 
  2.0f/4.0f, 
  1.0f/4.0f 
};

float4 main( float2 texcoord0 : TEXCOORD0 ) : COLOR0
{
  float4 sum = 0.0f;

  float2 Offset = PixelSize * -1;
  
  for( int i = 0; i < 3; i++ )
  {
    sum += coeff[i] * 1.1 * tex2D( Texture0, texcoord0 + Offset );
    Offset += PixelSize;
  }

  return clamp( sum, 0.0f, 1.0f );
}