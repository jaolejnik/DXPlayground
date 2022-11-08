#pragma once

#include "../pch.h"

constexpr const char *ShaderTypeToCstring[] =
    {
        "Diffuse",
        "Normal",
};

enum ShaderType
{
    Diffuse,
    Normal,
    COUNT,
};
struct ShaderStruct
{
    ID3D11VertexShader *vertexShader;
    ID3D11PixelShader *pixelShader;
    ID3D11InputLayout *inputLayout;
};
struct ConstantBufferStruct
{
    DirectX::XMFLOAT4X4 model;
    DirectX::XMFLOAT4X4 viewproj;
};

struct VertexPNC
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 normal;
    DirectX::XMFLOAT3 color;
};
