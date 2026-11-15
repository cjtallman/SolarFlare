
struct VERTEX
{
  float3 position : POSITION;  
  float3 normal	  : NORMAL;  
  float2 texcoord0: TEXCOORD0;
  float height    : TEXCOORD2;
  float3 tangent  : TEXCOORD6;
  float3 binormal : TEXCOORD7;
};

struct FRAGMENT
{
  float4 position     : POSITION;
  float2 texcoord0    : TEXCOORD0;
  float3 texcoord1    : TEXCOORD1;
  float3 normal	      : TEXCOORD2;
  float3 eye_position : TEXCOORD3;
  float3 light_dir    : TEXCOORD4;
  float  height       : TEXCOORD5;
};

struct PIXEL
{
  float4 color    : COLOR0;
};


float min_radius;
float max_radius;

FRAGMENT main( const VERTEX IN )
{
  FRAGMENT OUT;

  float3 normal = normalize(IN.normal);
  float3 binormal = normalize(IN.binormal);
  float3 tangent = normalize(IN.tangent);

  float h = (IN.height);
  float3 min_position = IN.position.xyz + normal * min_radius;
  float3 max_position = min_position + normal * (max_radius - min_radius);
  float3 new_position = min_position + h * (max_position - min_position);

  float3x3 objToTangentSpace;
	objToTangentSpace[0] = -tangent;
	objToTangentSpace[1] = binormal;
	objToTangentSpace[2] = normal;
	
  float3 eye_pos = mul( glstate.matrix.inverse.modelview[0], float4(0.0f,0.0f,0.0f,1.0f) ).xyz;
  float3 light = mul( glstate.matrix.inverse.modelview[0], glstate.light[0].position ).xyz;
  float3 light_direction = light - IN.position.xyz;

  OUT.position = mul( glstate.matrix.mvp, float4(new_position, 1.0f) );
  OUT.light_dir = mul(objToTangentSpace, light_direction);
  OUT.eye_position = mul(objToTangentSpace, eye_pos );
  OUT.normal = IN.normal;
  OUT.texcoord0 = IN.texcoord0;
  OUT.texcoord1 = normal;
  OUT.height = h;

  return OUT;
}

