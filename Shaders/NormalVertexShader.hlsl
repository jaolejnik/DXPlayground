cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
    matrix model;
    matrix viewproj;
};

struct vsIn
{
    float3 vPos   : POSITION;
    float3 vNormal: NORMAL;
    float3 vColor : COLOR;
};

struct vsOut 
{
    float4 Position : SV_POSITION;  
    float4 Color    : COLOR;
};

vsOut main(vsIn input) // main is the default function name
{
    vsOut Output;

    float4 pos = float4(input.vPos, 1.0f);

    // Transform the position from object space to homogeneous projection space
    pos = mul(pos, model);
    pos = mul(pos, viewproj);
    Output.Position = pos;

    // Just pass through the color data
    Output.Color = float4(input.vNormal, 1.0f);

    return Output;
}
