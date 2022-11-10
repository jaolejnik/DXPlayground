#include "Camera.h"
#include <cmath>

Camera::Camera(
    DirectX::XMFLOAT3 position,
    DirectX::XMFLOAT3 lookAt,
    DirectX::XMFLOAT3 up)

{
    m_position = position;
    m_lookAt = lookAt;
    m_up = up;
}

const DirectX::XMMATRIX Camera::GetViewProjectionMatrix(const float aspectRatioX)
{
    DirectX::XMMATRIX view =
        DirectX::XMMatrixLookAtRH(
            DirectX::XMVectorSet(m_position.x, m_position.y, m_position.z, 0.0f),
            DirectX::XMVectorSet(m_lookAt.x, m_lookAt.y, m_lookAt.z, 0.0f),
            DirectX::XMVectorSet(m_up.x, m_up.y, m_up.z, 0.0f));

    float aspectRatioY = aspectRatioX < (16.0f / 9.0f) ? aspectRatioX / (16.0f / 9.0f) : 1.0f;

    DirectX::XMMATRIX proj =
        DirectX::XMMatrixPerspectiveFovRH(
            2.0f * std::atan(std::tan(DirectX::XMConvertToRadians(70) * 0.5f) / aspectRatioY),
            aspectRatioX,
            0.01f,
            100.0f);

    return view * proj;
}

const DirectX::XMMATRIX Camera::GetViewProjectionMatrixInverse(const float aspectRatioX)
{
    return DirectX::XMMatrixInverse(
        nullptr,
        GetViewProjectionMatrix(aspectRatioX));
}
