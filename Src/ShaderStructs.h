#pragma once

#include "../pch.h"

struct ConstantBufferStruct
{
    DirectX::XMFLOAT4X4 model;
    DirectX::XMFLOAT4X4 viewproj;
};

// Assert that the constant buffer remains 16-byte aligned.
// static_assert((sizeof(ConstantBufferStruct) % 16) == 0, "Constant Buffer size must be 16-byte aligned");

struct VertexPositionColor
{
    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT3 color;
};
