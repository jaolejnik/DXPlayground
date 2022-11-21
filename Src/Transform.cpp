#include "Transform.h"

Transform::Transform()
{
    m_translation = {0.0f, 0.0f, 0.0f};
    m_rotation = {0.0f, 0.0f, 0.0f};
    m_scale = {1.0f, 1.0f, 1.0f};
}

void Transform::SetTranslation(DirectX::XMFLOAT3 v)
{
    m_translation = v;
}

void Transform::SetRotation(DirectX::XMFLOAT3 v)
{
    m_rotation = v;
}

void Transform::SetScale(DirectX::XMFLOAT3 v)
{
    m_scale = v;
}

void Transform::SetScaleUniform(float s)
{
    m_scale = {s, s, s};
}

DirectX::XMMATRIX Transform::GetTransformMatrix()
{
    DirectX::XMMATRIX tM = DirectX::XMMatrixTranslationFromVector(
        DirectX::XMVectorSet(m_translation.x, m_translation.y, m_translation.z, 0.0f));

    DirectX::XMMATRIX rM = DirectX::XMMatrixRotationAxis(X_AXIS, m_rotation.x) *
                           DirectX::XMMatrixRotationAxis(Y_AXIS, m_rotation.y) *
                           DirectX::XMMatrixRotationAxis(Z_AXIS, m_rotation.z);

    DirectX::XMMATRIX sM = DirectX::XMMatrixScalingFromVector(
        DirectX::XMVectorSet(m_scale.x, m_scale.y, m_scale.z, 0.0f));

    return sM * rM * tM;
}
