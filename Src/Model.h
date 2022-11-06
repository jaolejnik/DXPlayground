#pragma once

#include "DeviceResources.h"
#include "ShaderStructs.h"
#include "Transform.h"

class Model
{
private:
    std::string m_name;
    Transform m_transform;
    ConstantBufferStruct m_constantBufferData;
    std::vector<VertexPositionColor> m_vertices;
    std::vector<unsigned short> m_indices;

    // Direct3D device resources
    std::shared_ptr<DeviceResources> m_deviceResources;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;

    // HRESULT CreateFromFile(std::string path);
    void CreateCube();
    HRESULT InitializeBuffers();

public:
    Model(
        std::shared_ptr<DeviceResources> deviceResources,
        Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout,
        Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader,
        Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader);
    // Model(std::shared_ptr<DeviceResources> deviceResources,
    //       const std::string& filePath);
    ~Model() {}

    void Animate(UINT frameCount);
    void Update(UINT frameCount);
    void Render(DirectX::XMFLOAT4X4 viewproj);
};
