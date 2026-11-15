sampler2D particle_texture : TEXUNIT0;

struct VERTEX
{
  float4 position	: POSITION;
  float2 texcoord0	: TEXCOORD0;
  float size		: TEXCOORD1;
  float2 texoffset	: TEXCOORD2;
  float2 texsize	: TEXCOORD3;
  float3 normal		: NORMAL;
  float4 color		: COLOR0;
};

struct FRAGMENT
{
  float4 position     : POSITION;
  float2 texcoord0    : TEXCOORD0;
  float additivity    : TEXCOORD1;
  float4 color        : COLOR0;
};

struct PIXEL
{
	float4 color : COLOR0;
};


PIXEL main( const FRAGMENT IN )
{
	PIXEL OUT;

	float4 color = IN.color * tex2D(particle_texture, IN.texcoord0);
  float3 fgc = color.rgb * color.a;
  float alphac = color.a * (1 - IN.additivity);

  OUT.color = float4(fgc,alphac);
	return OUT;
}