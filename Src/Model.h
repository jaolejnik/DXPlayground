#pragma once

#include "DeviceResources.h"
#include "ShaderStructs.h"
#include "Transform.h"

class Model
{
private:
    const std::string m_name;
    Transform m_transform;
    ConstantBufferStruct m_constantBufferData;
    std::vector<VertexPNC> m_vertices;
    std::vector<uint16_t> m_indices;

    // Direct3D device resources
    std::shared_ptr<DeviceResources> m_deviceResources;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;

    bool CreateFromFile(const std::string &filePath);
    HRESULT InitializeBuffers();

public:
    Model(
        const std::string name,
        const std::string &filePath,
        std::shared_ptr<DeviceResources> deviceResources,
        Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout,
        Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader,
        Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader);
    ~Model() {}

    void Animate(UINT frameCount);
    void Update(UINT frameCount);
    void Render(DirectX::XMFLOAT4X4 viewproj);

    const char *GetName() { return m_name.c_str(); }
    Transform &GetTransform() { return m_transform; }
};
