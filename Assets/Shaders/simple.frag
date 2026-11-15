
sampler2D texture0;

float4 main( float2 texcoord0 : TEXCOORD0 ) : COLOR0
{
	return tex2D( texture0, texcoord0 );
}
