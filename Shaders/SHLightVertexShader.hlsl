#define COEFF_MAX_COUNT 16 

cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
    matrix model;
    matrix viewproj;
};

struct vsIn
{
    float3 position : POSITION;
    float3 normal   : NORMAL;
    float3 color    : COLOR;
};

struct vsOut 
{
    float4 position : SV_POSITION;  
    float4 color    : COLOR;
    float3 normal   : NORMAL;
};

vsOut main(vsIn input) // main is the default function name
{
    vsOut output;

    float4 pos = float4(input.position, 1.0f);
    pos = mul(pos, model);

    // works only for uniform scale
    output.normal = normalize(mul(float4(input.normal, 0.0f), model).xyz);
    output.position = mul(pos, viewproj);
    output.color = float4(input.color, 1.0f);

    return output;
}

