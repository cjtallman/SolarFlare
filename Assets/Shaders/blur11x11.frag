
sampler2D Texture0 : TEXUNIT0;

float2 PixelSize;

const float coeff[11] = 
{ 
  1.0f/1024.0f, 
  10.0f/1024.0f, 
  45.0f/1024.0f, 
  120.0f/1024.0f, 
  210.0f/1024.0f, 
  252.0f/1024.0f, 
  210.0f/1024.0f, 
  120.0f/1024.0f, 
  45.0f/1024.0f, 
  10.0f/1024.0f, 
  1.0f/1024.0f 
};

float4 main( float2 texcoord0 : TEXCOORD0 ) : COLOR0
{
  float4 sum = 0.0f;

  float2 Offset = PixelSize * -5;
  
  for( int i = 0; i < 11; i++ )
  {
    sum += coeff[i] * 1.1 * tex2D( Texture0, texcoord0 + Offset );
    Offset += PixelSize;
  }

  return clamp( sum, 0.0f, 1.0f );
}