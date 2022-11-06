#include <cmath>

#include "Renderer.h"

Renderer::Renderer(std::shared_ptr<DeviceResources> deviceResources)
{
    m_deviceResources = deviceResources;
    m_frameCount = 0;
}

// Create Direct3D shader resources by loading the .cso files.
HRESULT Renderer::CreateShaders()
{
    HRESULT hr = S_OK;

    // Use the Direct3D device to load resources into graphics memory.
    ID3D11Device *device = m_deviceResources->GetDevice();
    ID3DBlob *vertexShaderBlob = NULL;
    if (FAILED(
            D3DReadFileToBlob(L"VertexShader.cso", &vertexShaderBlob)))
    {
        printf("Failed to read vertex shader blob\n");
    }

    ID3DBlob *pixelShaderBlob = NULL;
    if (FAILED(D3DReadFileToBlob(L"PixelShader.cso", &pixelShaderBlob)))
    {
        printf("Failed to read pixel shader blob\n");
    }

    if (FAILED(device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(),
                                          vertexShaderBlob->GetBufferSize(),
                                          nullptr,
                                          m_pVertexShader.GetAddressOf())))
    {
        printf("Failed to create vertex shader\n");
    }

    if (FAILED(device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(),
                                         pixelShaderBlob->GetBufferSize(),
                                         nullptr,
                                         m_pPixelShader.GetAddressOf())))
    {
        printf("Failed to create pixel shader\n");
    }

    D3D11_INPUT_ELEMENT_DESC iaDesc[] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
             0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},

            {"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT,
             0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        };

    hr = device->CreateInputLayout(
        iaDesc,
        ARRAYSIZE(iaDesc),
        vertexShaderBlob->GetBufferPointer(),
        vertexShaderBlob->GetBufferSize(),
        &m_pInputLayout);

    return hr;
}

// Create the view matrix and create the perspective matrix.
void Renderer::CreateViewAndPerspective()
{
    // Use DirectXMath to create view and perspective matrices.
    DirectX::XMVECTOR eye = DirectX::XMVectorSet(0.0f, 0.7f, 1.5f, 0.f);
    DirectX::XMVECTOR at = DirectX::XMVectorSet(0.0f, -0.1f, 0.0f, 0.f);
    DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.f);

    DirectX::XMMATRIX view =
        DirectX::XMMatrixLookAtRH(
            eye,
            at,
            up);

    float aspectRatioX = m_deviceResources->GetAspectRatio();
    float aspectRatioY = aspectRatioX < (16.0f / 9.0f) ? aspectRatioX / (16.0f / 9.0f) : 1.0f;

    DirectX::XMMATRIX proj =
        DirectX::XMMatrixPerspectiveFovRH(
            2.0f * std::atan(std::tan(DirectX::XMConvertToRadians(70) * 0.5f) / aspectRatioY),
            aspectRatioX,
            0.01f,
            100.0f);

    DirectX::XMStoreFloat4x4(
        &m_constantBufferData.viewproj,
        DirectX::XMMatrixTranspose(
            view * proj));
}

// Create device-dependent resources for rendering.
void Renderer::CreateDeviceDependentResources()
{
    // Compile shaders using the Effects library.
    CreateShaders();
    m_models.emplace_back(
        new Model(
            m_deviceResources,
            m_pInputLayout,
            m_pVertexShader,
            m_pPixelShader));
}

void Renderer::CreateWindowSizeDependentResources()
{
    CreateViewAndPerspective();
}

// Update the scene.
void Renderer::Tick()
{
    m_frameCount++;

    for (Model *&model : m_models)
        model->Update(m_frameCount);

    if (m_frameCount == MAXUINT)
        m_frameCount = 0;
}

// Render the cube.
void Renderer::Render()
{
    for (Model *&model : m_models)
        model->Render(m_constantBufferData.viewproj);
}

Renderer::~Renderer()
{
    for (Model *&model : m_models)
        delete model;
}
