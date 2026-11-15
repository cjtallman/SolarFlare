
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

float4 Ambient : state.light[0].ambient;
float4 Diffuse : state.light[0].diffuse;
float4 Specular : state.light[0].specular;

PIXEL main( const FRAGMENT IN )
{  
  PIXEL OUT;

  float4 Ks = Specular;
  float4 Kd = Diffuse;
  float4 Ka = Ambient;

  float4 Kt = .5;// * tex2D(texture0, IN.texcoord0.xy);
  float shiny = 100;
  
  float3 N = IN.normal;
  float3 L = normalize(IN.light_dir);
  float3 E = normalize(IN.eye_position);
  float3 H = normalize(E + L);

  float diff = saturate(dot(N, L));
  float spec = pow(saturate(dot(N, H)), shiny);

  OUT.color = Kt * (Ka + Kd * diff) + Ks * spec;
  OUT.color.a = 1;
  return OUT;
}
