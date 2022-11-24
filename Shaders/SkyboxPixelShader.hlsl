TextureCube skyboxTexture : register(t0);
SamplerState textureSampler : register(s0);

struct psIn
{
	float4 position : SV_POSITION;
	float3 texcoord : TEXCOORD;
};

float4 main(psIn input) : SV_TARGET
{
	return skyboxTexture.Sample(textureSampler, input.texcoord);
}
