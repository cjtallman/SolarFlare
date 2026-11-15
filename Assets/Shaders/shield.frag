
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
  float3 texcoord0: TEXCOORD0;
  float3 normal	  : TEXCOORD1;
  float3 E        : TEXCOORD2;
  float3 L        : TEXCOORD3;
  float4 color    : COLOR0;
};

sampler2D diffuse_texture : TEXUNIT0;

float4 model_color;
float4 glow_color;
float glow_amount;

float4 Ambient        : state.light[0].ambient;
float4 Diffuse        : state.light[0].diffuse;
float4 Specular       : state.light[0].specular;

float4 main( const FRAGMENT IN ) : COLOR0
{  
  float4 Ka = .25f * Ambient;
  float4 Kd = Diffuse;
  float4 Ks = 5 * Specular;
  float4 Kt = tex2D( diffuse_texture, IN.texcoord0 );
  float shiny = 200;
  
  float3 N = normalize(IN.normal);

  float3 L = normalize(IN.L);
  float3 E = normalize(IN.E);
  float3 H = normalize(E + L);

  float spec = pow(saturate(dot(N, H)), shiny);
  
  float4 color = Kt + Kd * Ks*spec;
  color *= glow_amount * glow_color * Kt.a;
  return float4(color.rgb * Kd.a, .2*glow_amount*Kt.a);
}
