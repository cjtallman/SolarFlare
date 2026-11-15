sampler2D Texture0 : TEXUNIT0;

float4 main( const float2 texcoord0 : TEXCOORD0, const float4 color : COLOR0 ) : COLOR0
{
    return 1.5f * color * tex2D(Texture0, texcoord0);
}