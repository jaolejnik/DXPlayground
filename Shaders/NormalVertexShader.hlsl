cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
    matrix model;
    matrix viewproj;
};

struct vsIn
{
    float3 position: POSITION;
    float3 normal: NORMAL;
    float3 color: COLOR;
};

struct vsOut 
{
    float4 position: SV_POSITION;
    float3 normal: NORMAL;
};

vsOut main(vsIn input) 
{
    vsOut output;

    float4 pos = float4(input.position, 1.0f);

    pos = mul(pos, model);
    pos = mul(pos, viewproj);

    output.position = pos;
    output.normal = input.normal;

    return output;
}
