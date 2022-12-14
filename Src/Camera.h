#pragma once

#include "../pch.h"
#include "Transform.h"

class Camera
{
private:
    DirectX::XMFLOAT3 m_position;
    DirectX::XMFLOAT3 m_lookAt;
    DirectX::XMFLOAT3 m_up;

public:
    Camera(
        DirectX::XMFLOAT3 position,
        DirectX::XMFLOAT3 lookAt,
        DirectX::XMFLOAT3 up);

    void Orbit(UINT framecount, float radius, DirectX::XMFLOAT3 lookAt);

    const DirectX::XMMATRIX GetViewProjectionMatrix(const float aspectRatioX);
    const DirectX::XMMATRIX GetViewProjectionMatrixInverse(const float aspectRatioX);
    const DirectX::XMFLOAT3 GetPosition() { return m_position; }
    const DirectX::XMFLOAT3 GetLookAt() { return m_lookAt; }
};
