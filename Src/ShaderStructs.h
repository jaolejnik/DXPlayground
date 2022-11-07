#pragma once

#include "../pch.h"

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
