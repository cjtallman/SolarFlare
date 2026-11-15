
struct VERTEX
{
  float3 position : POSITION;
  float3 normal	  : NORMAL;
  float3 texcoord0: TEXCOORD0;
  float4 color	  : COLOR0;
};

struct FRAGMENT
{
  float4 position : POSITION;
  float3 normal	  : TEXCOORD1;
  float3 texcoord0: TEXCOORD0;
  float3 light_dir: TEXCOORD3;
  float3 eye_position : TEXCOORD2;
  float4 color    : COLOR0;
};

struct PIXEL
{
  float4 color    : COLOR0;
};


FRAGMENT main( const VERTEX IN )
{
  FRAGMENT OUT;

  float3 normal = normalize(IN.normal);

  float3 eye_pos = mul( glstate.matrix.inverse.modelview[0], float4(0.0f,0.0f,0.0f,1.0f) ).xyz;
  float3 light = mul( glstate.matrix.inverse.modelview[0], glstate.light[0].position ).xyz;
  float3 light_direction = light - IN.position.xyz;

  OUT.position = mul( glstate.matrix.mvp, float4(IN.position, 1.0f) );
  OUT.light_dir = light_direction;
  OUT.eye_position = eye_pos;
  OUT.normal = IN.normal;
  OUT.texcoord0 = IN.texcoord0;
  OUT.color = IN.color;

  return OUT;
}
