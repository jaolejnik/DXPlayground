#pragma once

#include "../pch.h"

static const DirectX::XMVECTOR X_AXIS = {1.0f, 0.0f, 0.0f, 0.0f};
static const DirectX::XMVECTOR Y_AXIS = {0.0f, 1.0f, 0.0f, 0.0f};
static const DirectX::XMVECTOR Z_AXIS = {0.0f, 0.0f, 1.0f, 0.0f};

class Transform
{
private:
    DirectX::XMFLOAT3 m_translation;
    DirectX::XMFLOAT3 m_rotation;
    DirectX::XMFLOAT3 m_scale;

public:
    Transform();

    void SetTranslation(DirectX::XMFLOAT3 v);
    void SetRotation(DirectX::XMFLOAT3 v);
    void SetScale(DirectX::XMFLOAT3 v);

    DirectX::XMMATRIX GetTransformMatrix();
};
