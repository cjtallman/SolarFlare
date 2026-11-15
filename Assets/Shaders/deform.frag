struct VERTEX
{
  float4 position : POSITION;
  float2 texcoord0: TEXCOORD0;
  float scale	    : TEXCOORD1;
};

struct FRAGMENT
{
  float3 position : POSITION;
  float2 texcoord0: TEXCOORD0;
  float scale: TEXCOORD1;
};

sampler2D texture0 : TEXUNIT0;

float4 main( const FRAGMENT IN ) : COLOR0
{
  
  float scale = IN.scale / 18;
  float3 color = float3(1,1,1) - tex2D( texture0, IN.texcoord0 ).rgb;
  float gray = dot(scale * color, float3(1,1,1));
  float3 outcolor = saturate(float3(1 - gray, 1 - gray, 1 - gray));
	return float4(outcolor.rgb, 1);
	
	/*
  float3 color = tex2D( texture0, IN.texcoord0 ).rgb;
	return float4(color.rgb, 1);
	*/
}
