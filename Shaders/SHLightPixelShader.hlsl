#define COEFF_MAX_COUNT 16 

cbuffer SceneConstantBuffer : register(b1)
{
	float4 lightPosition;
	float4 shCoefficients[COEFF_MAX_COUNT];
};

struct psIn
{
	float4 position : SV_POSITION;
	float4 color    : COLOR;
	float3 normal   : NORMAL;
};


// basis calculated from precomputed values for optimazation
float Ylm(int i, float3 normal)
{
	if (i == 0) // l = 0, m =  0
		return 0.282095;
	if (i == 1) // l = 1, m = -1
		return 0.488603 * normal.y;
	if (i == 2) // l = 1, m =  0
		return 0.488603 * normal.z;
	if (i == 3) // l = 1, m =  1
		return 0.488603 * normal.x;
	if (i == 4) // l = 2, m = -2
		return 1.092548 * normal.y * normal.x;
	if (i == 5) // l = 2, m = -1
		return 1.092548 * normal.y * normal.z;
	if (i == 6) // l = 2, m =  0
		return 0.315392 * (3 * normal.z * normal.z - 1);
	if (i == 7) // l = 2, m =  1
		return 1.092548 * normal.x * normal.z;
	if (i == 8) // l = 2, m =  2
		return 0.546274 * (normal.x * normal.x - normal.y * normal.y);

	return 0;
}

float4 decode(float3 normal, float4 coeffs[COEFF_MAX_COUNT])
{
	float4 result = 0.0f;
	for (int i = 0; i < 9; i++)
	{
		result += coeffs[i] * Ylm(i, normal);
	}

	return clamp(result, 0.0, 1.0);
}

float4 main(psIn input) : SV_TARGET
{
	float x = input.normal.x;
	float y = input.normal.y;
	float z = input.normal.z;

	float4 result = decode(float3(x, y, z), shCoefficients);
	float3 diffuse = input.color.xyz;
	float3 finalcolor = diffuse * result;

	return float4(finalcolor, 1.0f);
}
