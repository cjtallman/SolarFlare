sampler2D Texture0 : TEXUNIT0;

float4 grad_top;
float4 grad_bottom;

float4 main( const float2 texcoord0 : TEXCOORD0, const float4 color : COLOR0 ) : COLOR0
{
  float4 diffuse = tex2D(Texture0, texcoord0) * color;
  float y = ((int)(texcoord0.y * 512) % 32) / 16.0f;
  return diffuse * lerp(grad_top, grad_bottom, y*.8f);
}