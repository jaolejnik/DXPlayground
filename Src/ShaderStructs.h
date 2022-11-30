#pragma once

#include "../pch.h"
#include "Sh.h"

constexpr char *ShaderTypeToCstring[] =
    {
        "SHLight",
        "Diffuse",
        "Normal",
        "Skybox",
};

enum class ShaderType
{
    SHLight,
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

static_assert((sizeof(TransformBufferStruct) % 16) == 0, "Transform Constant Buffer size must be 16-byte aligned");

struct SceneBufferStruct
{
    DirectX::XMFLOAT4 lightPosition;
    DirectX::XMFLOAT4 shCoefficients[16];
};

static_assert((sizeof(SceneBufferStruct) % 16) == 0, "Scene Constant Buffer size must be 16-byte aligned");

struct VertexPNC
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 normal;
    DirectX::XMFLOAT3 color;
};
