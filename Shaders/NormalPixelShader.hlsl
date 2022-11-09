struct psIn
{
    float4 position: SV_POSITION;
    float3 normal: NORMAL;
};

float4 main(psIn input) : SV_TARGET
{
	return float4((normalize(input.normal) + 1.0) / 2.0, 1.0);
}
