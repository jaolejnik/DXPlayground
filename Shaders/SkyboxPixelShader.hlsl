struct psIn
{
	float4 position : SV_POSITION;  
	float4 color    : COLOR;       
    float3 normal   : NORMAL;
};

float4 main(psIn input) : SV_TARGET
{
	float3 diffuse = input.color.xyz;

	return float4(diffuse, 0.0f); 
}
