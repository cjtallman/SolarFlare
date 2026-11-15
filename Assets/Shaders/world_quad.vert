
struct VERTEX
{
	float4 position : POSITION;
	float2 texcoord0 : TEXCOORD0;
	float4 color : COLOR0;
};

struct FRAGMENT
{
	float4 position : POSITION;
	float2 texcoord0 : TEXCOORD0;
	float4 color : COLOR0;
};

FRAGMENT main( const VERTEX IN )
{
  FRAGMENT OUT;
  
  OUT.position = mul( glstate.matrix.mvp, IN.position );
  OUT.texcoord0 = IN.texcoord0;
  OUT.color = IN.color;

  return OUT;
}
