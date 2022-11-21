#pragma once

#include "../pch.h"

constexpr const char *ShaderTypeToCstring[] =
    {
        "Diffuse",
        "Normal",
        "Skybox",
};

enum class ShaderType
{
    Diffuse,
    Normal,
    Skybox,
    COUNT,
};

struct ShaderStruct
{
    ID3D11VertexShader *vertexShader;
    ID3D11PixelShader *pixelShader;
    ID3D11InputLayout *inputLayout;
};

struct TransformBufferStruct
{
    DirectX::XMFLOAT4X4 model;
    DirectX::XMFLOAT4X4 viewproj;
};

struct SceneBufferStruct
{
    DirectX::XMFLOAT4 cameraPosition;
    DirectX::XMFLOAT4 lightPosition;
};

struct VertexPNC
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 normal;
    DirectX::XMFLOAT3 color;
};
