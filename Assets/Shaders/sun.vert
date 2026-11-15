
struct VERTEX
{
  float3 position     : POSITION; 
  float3 normal	      : NORMAL; 
  float3 texcoord0    : TEXCOORD0; 
  float  height       : TEXCOORD2;
};

struct FRAGMENT
{
  float4 position     : POSITION;
  float3 texcoord0    : TEXCOORD0;
};

struct PIXEL
{
  float4 color        : COLOR0;
};

float PlanetRadius : PlanetRadius;
float min_radius;
float max_radius;

FRAGMENT main( const VERTEX IN )
{
  FRAGMENT OUT;

  float3 normal = normalize(IN.normal);

  float h = IN.height;
  float3 min_position = IN.position.xyz + normal * min_radius;
  OUT.position = mul( glstate.matrix.mvp, float4(min_position, 1.0f) );
  OUT.texcoord0 = IN.texcoord0;

  return OUT;
}

