
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

sampler2D height_texture : TEXUNIT0;
sampler1D color_texture : TEXUNIT1;

PIXEL main( const FRAGMENT IN )
{  
  PIXEL OUT;
  float4 sun_color = tex1D(color_texture, tex2D(height_texture, IN.texcoord0).r);
  OUT.color = 3.0f * (sun_color);
  OUT.color.a = 1000;
  return OUT;
}