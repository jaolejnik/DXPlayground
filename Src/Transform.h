#pragma once

#include "../pch.h"

constexpr DirectX::XMVECTOR X_AXIS = {1.0f, 0.0f, 0.0f, 0.0f};
constexpr DirectX::XMVECTOR Y_AXIS = {0.0f, 1.0f, 0.0f, 0.0f};
constexpr DirectX::XMVECTOR Z_AXIS = {0.0f, 0.0f, 1.0f, 0.0f};

class Transform
{
public:
    DirectX::XMFLOAT3 m_translation;
    DirectX::XMFLOAT3 m_rotation;
    DirectX::XMFLOAT3 m_scale;

    bool m_useUniformScale = true;

    Transform();

    void SetTranslation(DirectX::XMFLOAT3 v);
    void SetRotation(DirectX::XMFLOAT3 v);
    void SetScale(DirectX::XMFLOAT3 v);
    void SetScaleUniform(float s);

    const DirectX::XMFLOAT3 GetTranslation() { return m_translation; }
    const DirectX::XMFLOAT3 GetRotation() { return m_rotation; }
    const DirectX::XMFLOAT3 GetScale() { return m_scale; }

    DirectX::XMMATRIX GetTransformMatrix();
};
