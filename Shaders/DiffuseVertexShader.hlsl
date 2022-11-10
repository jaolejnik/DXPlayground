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
    float3 position : POSITION;
    float3 normal   : NORMAL;
    float3 color    : COLOR;
};

struct vsOut 
{
    float4 position : SV_POSITION;  
    float4 color    : COLOR;
    float3 normal   : NORMAL;
    float3 viewDir  : POSITION0;
    float3 lightDir : POSITION1;
};

vsOut main(vsIn input) // main is the default function name
{
    vsOut output;

    float4 pos = float4(input.position, 1.0f);
    pos = mul(pos, model);

    output.viewDir = normalize(cameraPosition - pos); 
    output.lightDir = normalize(lightPosition - pos); 
    // works only for uniform scale
    output.normal = normalize(mul(float4(input.normal, 0.0f), model).xyz);
    output.position = mul(pos, viewproj);
    output.color = float4(input.color, 1.0f);

    return output;
}
