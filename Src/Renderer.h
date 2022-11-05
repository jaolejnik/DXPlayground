#pragma once

#include "DeviceResources.h"
#include "ShaderStructs.h"

class Renderer
{
private:
    std::shared_ptr<DeviceResources> m_deviceResources;
    ConstantBufferStruct m_constantBufferData;
    unsigned int m_indexCount;
    unsigned int m_frameCount;

    // Direct3D device resources
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayoutExtended;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;

    HRESULT CreateShaders();
    HRESULT CreateCube();
    void CreateViewAndPerspective();

public:
    Renderer(std::shared_ptr<DeviceResources> deviceResources);
    ~Renderer() {}

    float m_clearColor[4] = {0.0f, 0.0f, 0.f, 1.0f};

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();
    void Update();
    void Render();
};
