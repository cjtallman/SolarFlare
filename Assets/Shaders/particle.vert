

struct VERTEX
{
  float4 position	  : POSITION;
  float2 texcoord0	: TEXCOORD0;
  float size        : TEXCOORD1;
  float2 texoffset	: TEXCOORD2;
  float2 texsize	  : TEXCOORD3;
  float additivity  : TEXCOORD4;
  float3 normal		  : NORMAL;
  float4 color		  : COLOR0;
};

struct FRAGMENT
{
  float4 position     : POSITION;
  float2 texcoord0    : TEXCOORD0;
  float additivity    : TEXCOORD1;
  float4 color        : COLOR0;
};

struct PIXEL
{
	float4 color : COLOR0;
};

FRAGMENT main( const VERTEX IN )
{
  FRAGMENT OUT;
  
  float2 texcoord = IN.texcoord0 * IN.texsize + IN.texoffset;
  float3 position = IN.normal + 
    IN.size * (IN.position.x) * glstate.matrix.modelview[0]._m00_m01_m02 + 
    IN.size * (IN.position.y) * glstate.matrix.modelview[0]._m10_m11_m12;

  OUT.position = mul( glstate.matrix.mvp, float4(position, 1) );
  OUT.texcoord0 = texcoord;
  OUT.color = IN.color;
  OUT.additivity = IN.additivity;

  return OUT;
}
