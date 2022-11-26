#define COEFF_MAX_COUNT 16 
#define PI 3.141592654f

const static float factorial[35] = {
	1.0,
	1.0,
	2.0,
	6.0,
	24.0,
	120.0,
	120.0,
	720.0,
	5040.0,
	40320.0,
	362880.0,
	3628800.0,
	39916800.0,
	479001600.0,
	6227020800.0,
	87178291200.0,
	1307674368000.0,
	20922789888000.0,
	355687428096000.0,
	6402373705728000.0,
	121645100408832000.0,
	2432902008176640000.0,
	51090942171709440000.0,
	1124000727777607680000.0,
	25852016738884976640000.0,
	620448401733239439360000.0,
	15511210043330985984000000.0,
	403291461126605635584000000.0,
	10888869450418352160768000000.0,
	304888344611713860501504000000.0,
	8841761993739701954543616000000.0,
	265252859812191058636308480000000.0,
	8222838654177922817725562880000000.0,
	263130836933693530167218012160000000.0,
	8683317618811886495518194401280000000.0,
};

const static float A[5] = {
	PI,
	2.094395,
	0.785398,
	0.0,
	0.049087,
};

// ---- BUFFERS AND STRUCTS ---- 
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

// ---- FUNCTIONS ---- 
// renormalisation constatn for SH function
float K(int l, int m)
{
	float tmp = ((2.0 * l + 1.0) * factorial[l - m]) /
		(4.0 * PI * factorial[l + m]);
	return sqrt(tmp);
}

// evaluate an Associated Legendre Polynomial at x
float P(int l, int m, float x)
{
	float pmm = 1.0;
	if (m > 0)
	{
		float somx2 = sqrt((1.0 - x) * (1.0 + x));
		float fact = 1.0;
		for (int i = 1; i <= m; i++)
		{
			pmm *= (-fact) * somx2;
			fact += 2.0;
		}
	}

	if (l == m)
		return pmm;

	float pmmp1 = x * (2.0 * m + 1.0) * pmm;
	if (l == m + 1)
		return pmmp1;

	float pll = 0.0;
	for (int ll = m + 2; ll <= l; ll++)
	{
		pll = ((2.0 * ll - 1.0) * x * pmmp1 - (ll + m - 1.0) * pmm) / (ll - m);
		pmm = pmmp1;
		pmmp1 = pll;
	}

	return pll;
}

float evaluateSH(int l, int m, float theta, float phi)
{
	if (m == 0)
		return K(l, 0) * P(l, m, cos(theta));
	else if (m > 0)
		return sqrt(2) * K(l, m) * cos(m * phi) * P(l, m, cos(theta));
	else
		return sqrt(2) * K(l, -m) * sin(-m * phi) * P(l, -m, cos(theta));
}

float Ylm(int i, float theta, float phi)
{
	int l = floor((sqrt(i)));
	int m = i - l * (l + 1);

	return evaluateSH(l, m, theta, phi);
}

float4 decode(float theta, float phi, float4 coeffs[COEFF_MAX_COUNT])
{
	float4 result = 0.0f;
	for (int i = 0; i < 9; i++)
	{
		result += coeffs[i] * Ylm(i, theta, phi);
	}

	return clamp(result, 0.0, 1.0);
}

// ---- MAIN ----
float4 main(psIn input) : SV_TARGET
{
	float x = input.normal.x;
	float y = input.normal.y;
	float z = input.normal.z;

	float phi= atan2(y, x);
	float theta= atan2(sqrt(x * x + y * y), z);
	float4 result = decode(theta, phi, shCoefficients);

	float3 diffuse = input.color.xyz;
	float3 finalcolor = diffuse * result; 

	return float4(finalcolor, 1.0f);
}
