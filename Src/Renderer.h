#pragma once

#include "DeviceResources.h"
#include "ShaderStructs.h"
#include "Model.h"

class Renderer
{
private:
    std::shared_ptr<DeviceResources> m_deviceResources;
    ConstantBufferStruct m_constantBufferData;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
    unsigned int m_frameCount;
    std::vector<Model *> m_models;

    HRESULT CreateShaders();
    void CreateViewAndPerspective();

public:
    Renderer(std::shared_ptr<DeviceResources> deviceResources);
    ~Renderer();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();
    void Tick();
    void Render();

    std::vector<Model *> &GetModels() { return m_models; }
};
