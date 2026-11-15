
struct VERTEX
{
  float4 position : POSITION;
  float2 texcoord0: TEXCOORD0;
  float scale	    : TEXCOORD1;
};

struct FRAGMENT
{
  float3 position : POSITION;
  float2 texcoord0: TEXCOORD0;
  float scale: TEXCOORD1;
};

FRAGMENT main( const VERTEX IN )
{
  FRAGMENT OUT;
  OUT.position = mul( glstate.matrix.mvp, IN.position ).xyz;
  OUT.texcoord0 = IN.texcoord0;
  OUT.scale = IN.scale;

  return OUT;
}
