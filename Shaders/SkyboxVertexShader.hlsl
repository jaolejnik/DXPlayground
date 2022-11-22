cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
    matrix model;
    matrix viewproj;
};

struct vsIn
{
    float3 position : POSITION;
    float3 normal   : NORMAL;
    float3 texcoord : TEXCOORD; 
};

struct vsOut 
{
    float4 position : SV_POSITION;  
    float3 texcoord : TEXCOORD; 
};

vsOut main(vsIn input) // main is the default function name
{
    vsOut output;

    float4 pos = float4(input.position, 1.0f);
    pos = mul(pos, model);

    // works only for uniform scale
    output.position = mul(pos, viewproj);
    output.texcoord = input.position;

    return output;
}
