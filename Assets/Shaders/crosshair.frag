

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

float4 model_color;
float4 glow_color;
float glow_amount;

sampler2D texture0 : TEXUNIT0;

PIXEL main( const FRAGMENT IN )
{  
  PIXEL OUT;

  float4 Kt = model_color * tex2D(texture0, IN.texcoord0.xy);
  
  OUT.color = glow_amount * glow_color + Kt;
  OUT.color.a = model_color.a;
  
  return OUT;
}