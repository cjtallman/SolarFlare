
struct VERTEX
{
  float3 position : POSITION;
  float3 normal	  : NORMAL;
  float4 color	  : COLOR0;
  float3 texcoord0: TEXCOORD0;
  float3 tangent  : TEXCOORD6;
  float3 binormal : TEXCOORD7;
};

struct FRAGMENT
{
  float4 position : POSITION;
  float3 texcoord0: TEXCOORD0;
  float3 L        : TEXCOORD3;
  float3 E        : TEXCOORD2;
  float4 color    : COLOR0;
};

FRAGMENT main( const VERTEX IN )
{
  FRAGMENT OUT;

  float3 normal = normalize(IN.normal);
  float3 binormal = normalize(IN.binormal);
  float3 tangent = normalize(IN.tangent);

  float3x3 objToTangentSpace = float3x3( tangent, binormal, normal );

  float3 E = mul( glstate.matrix.inverse.modelview[0], float4(0.0f,0.0f,0.0f,1.0f) ).xyz;
  float3 light = mul( glstate.matrix.inverse.modelview[0], glstate.light[0].position ).xyz;
  float3 L = light - IN.position.xyz;

  OUT.position = mul( glstate.matrix.mvp, float4(IN.position, 1.0f) );
  OUT.L = mul(objToTangentSpace, L);
  OUT.E = mul(objToTangentSpace, E);
  OUT.texcoord0 = IN.texcoord0;
  OUT.color = IN.color;

  return OUT;
}

