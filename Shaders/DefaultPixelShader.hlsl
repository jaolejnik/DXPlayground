struct psIn
{
	float4 position : SV_POSITION;  
	float4 color    : COLOR;       
    float3 normal   : NORMAL;
};

float4 main(psIn input) : SV_TARGET
{
	return float4(input.color.xyz, 1.0f); 
}
