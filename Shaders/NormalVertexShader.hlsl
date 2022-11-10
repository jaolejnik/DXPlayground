cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
    matrix model;
    matrix viewproj;
};

cbuffer SceneConstantBuffer : register(b1)
{
    float3 cameraPosition;
    float3 lightPosition;
}

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
    // works only for uniform scale
    output.normal = normalize(mul(float4(input.normal, 0.0f), model).xyz);

    return output;
}
