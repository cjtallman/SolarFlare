
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

sampler2D diffuse_texture : TEXUNIT0;
sampler2D normal_texture 	: TEXUNIT1;

float4 Ambient        : state.light[0].ambient;
float4 Diffuse        : state.light[0].diffuse;
float4 Specular       : state.light[0].specular;

float4 main( const FRAGMENT IN ) : COLOR0
{  
  float4 Ka = .25f * Ambient;
  float4 Kd = Diffuse;
  float4 Ks = Specular;
  float4 Kt = tex2D( diffuse_texture, IN.texcoord0 );
  float shiny = 1000;

  float3 N = tex2D( normal_texture, IN.texcoord0 ).rgb;
  N = normalize( (N - 0.5f) * 2.0f );

  float3 L = normalize(IN.L);
  float3 E = normalize(IN.E);
  float3 H = normalize(E + L);

  float diff = saturate(dot(N, L));
  float spec = pow(saturate(dot(N, H)), shiny);

  return float4(Kt * (Ka + Kd * diff + Ks * spec).rgb, 1);
}
