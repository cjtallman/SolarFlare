
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



sampler2D height_texture 	: TEXUNIT0;
sampler1D color_texture 	: TEXUNIT1;
samplerCUBE deform_texture: TEXUNIT2;

float texwidth;
float texheight;



float4 Ambient : state.light[0].ambient;
float4 Diffuse : state.light[0].diffuse;
float4 Specular : state.light[0].specular;

PIXEL main( const FRAGMENT IN )
{  
  PIXEL OUT;

  float4 Ka = Ambient;
  float4 Kd = Diffuse;
  float4 Ks = Specular;

  float pixu = 1.0f / texwidth;
  float pixv = 1.0f / texheight;

  float noise_height = tex2D(height_texture, IN.texcoord0).r;
  float4 deform_color = texCUBE(deform_texture, IN.texcoord1);
  
  float h = deform_color.r * noise_height;
  float4 Kt = lerp( tex1D(color_texture, h ),  tex1D(color_texture, IN.height), 1-noise_height );
  float shiny = 200;
  
  float north = tex2D(height_texture, IN.texcoord0 + 1.0 * float2(0,pixv) ).r;
  float south = tex2D(height_texture, IN.texcoord0 - 1.0 * float2(0,pixv) ).r;
  float west = tex2D(height_texture, IN.texcoord0 - 1.0 * float2(pixu,0) ).r;
  float east = tex2D(height_texture, IN.texcoord0 + 1.0 * float2(pixu,0) ).r;
  
  float dx = 3 * (east - west);
  float dy = 3 * (south - north);
  float3 N = normalize(float3(dx,dy,1));
  float3 L = normalize(IN.light_dir);
  float3 E = normalize(IN.eye_position);
  float3 H = normalize(E + L);
  float diff = saturate(dot(N, L));
  float spec = pow(saturate(dot(N, H)), shiny);
  OUT.color  = float4(5000,.4*noise_height,.15*noise_height*noise_height,5000) * pow(1-h,50000*noise_height*IN.height) + .75f * h * Kt * (Ka + Kd * diff + Ks * spec);
  
  return OUT;
}
