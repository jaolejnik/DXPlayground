struct psIn
{
	float4 position : SV_POSITION;  
	float4 color    : COLOR;       
    float3 normal   : NORMAL;
    float3 lightDir : POSITION0;
};

float4 main(psIn input) : SV_TARGET
{
	float3 diffuse = input.color.xyz * max(dot(input.normal, input.lightDir), 0.0f);

	return float4(diffuse, 1.0f); 
}
