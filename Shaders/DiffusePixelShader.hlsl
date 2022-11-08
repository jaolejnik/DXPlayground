struct psIn
{
	float4 Position : SV_POSITION;  // interpolated vertex position (system value)
	float4 Color    : COLOR0;       // interpolated diffuse color
};

float4 main(psIn Input) : SV_TARGET
{
	float4 Output;

	Output = Input.Color;

	return Output;
}
