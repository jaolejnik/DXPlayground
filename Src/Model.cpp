#include "Model.h"

Model::Model(
    std::shared_ptr<DeviceResources> deviceResources,
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout,
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader,
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader)
{
    m_deviceResources = deviceResources;
    m_pInputLayout = inputLayout;
    m_pVertexShader = vertexShader;
    m_pPixelShader = pixelShader;
    CreateCube();
    InitializeBuffers();
}

void Model::CreateCube()
{
    m_vertices =
        {
            {
                DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f),
                DirectX::XMFLOAT3(0, 0, 0),
            },
            {
                DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f),
                DirectX::XMFLOAT3(0, 0, 1),
            },
            {
                DirectX::XMFLOAT3(-0.5f, 0.5f, -0.5f),
                DirectX::XMFLOAT3(0, 1, 0),
            },
            {
                DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f),
                DirectX::XMFLOAT3(0, 1, 1),
            },

            {
                DirectX::XMFLOAT3(0.5f, -0.5f, -0.5f),
                DirectX::XMFLOAT3(1, 0, 0),
            },
            {
                DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f),
                DirectX::XMFLOAT3(1, 0, 1),
            },
            {
                DirectX::XMFLOAT3(0.5f, 0.5f, -0.5f),
                DirectX::XMFLOAT3(1, 1, 0),
            },
            {
                DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f),
                DirectX::XMFLOAT3(1, 1, 1),
            },
        };

    m_indices =
        {
            0,
            2,
            1, // -x
            1,
            2,
            3,

            4,
            5,
            6, // +x
            5,
            7,
            6,

            0,
            1,
            5, // -y
            0,
            5,
            4,

            2,
            6,
            7, // +y
            2,
            7,
            3,

            0,
            4,
            6, // -z
            0,
            6,
            2,

            1,
            3,
            7, // +z
            1,
            7,
            5,
        };
}

HRESULT Model::InitializeBuffers()
{
    HRESULT hr = S_OK;

    // Use the Direct3D device to load resources into graphics memory.
    ID3D11Device *device = m_deviceResources->GetDevice();

    // Create vertex buffer:
    CD3D11_BUFFER_DESC vDesc(
        (UINT)(sizeof(VertexPositionColor) * m_vertices.size()),
        D3D11_BIND_VERTEX_BUFFER);

    D3D11_SUBRESOURCE_DATA vData = {m_vertices.data()};
    hr = device->CreateBuffer(
        &vDesc,
        &vData,
        &m_pVertexBuffer);

    // Create index buffer:
    CD3D11_BUFFER_DESC iDesc(
        (UINT)(sizeof(unsigned short) * m_indices.size()),
        D3D11_BIND_INDEX_BUFFER);

    D3D11_SUBRESOURCE_DATA iData = {m_indices.data()};
    hr = device->CreateBuffer(
        &iDesc,
        &iData,
        &m_pIndexBuffer);

    // Create constant buffer:
    CD3D11_BUFFER_DESC cbDesc(
        sizeof(ConstantBufferStruct),
        D3D11_BIND_CONSTANT_BUFFER);

    hr = device->CreateBuffer(
        &cbDesc,
        nullptr,
        m_pConstantBuffer.GetAddressOf());

    return hr;
}

void Model::Animate(UINT frameCount)
{
    // Rotate the cube 1 degree per frame.

    m_transform.SetTranslation(
        {
            ((float)frameCount) / 100.0f,
            0.0f,
            0.0f,
        });

    m_transform.SetRotation(
        {
            0.0f,
            DirectX::XMConvertToRadians((float)frameCount),
            0.0f,
        });
}

void Model::Update(UINT frameCount)
{
    // Animate(frameCount);

    DirectX::XMStoreFloat4x4(
        &m_constantBufferData.model,
        DirectX::XMMatrixTranspose(m_transform.GetTransformMatrix()));
}

void Model::Render(DirectX::XMFLOAT4X4 viewproj)
{
    // Use the Direct3D device context to draw.
    ID3D11DeviceContext *context = m_deviceResources->GetDeviceContext();

    // Set up the vertex shader stage.
    context->VSSetShader(
        m_pVertexShader.Get(),
        nullptr,
        0);

    // Set up the pixel shader stage.
    context->PSSetShader(
        m_pPixelShader.Get(),
        nullptr,
        0);

    context->VSSetConstantBuffers(
        0,
        1,
        m_pConstantBuffer.GetAddressOf());

    m_constantBufferData.viewproj = viewproj;
    context->UpdateSubresource(
        m_pConstantBuffer.Get(),
        0,
        nullptr,
        &m_constantBufferData,
        0,
        0);

    // Set up the IA stage by setting the input topology and layout.
    UINT stride = sizeof(VertexPositionColor);
    UINT offset = 0;

    context->IASetVertexBuffers(
        0,
        1,
        m_pVertexBuffer.GetAddressOf(),
        &stride,
        &offset);

    context->IASetIndexBuffer(
        m_pIndexBuffer.Get(),
        DXGI_FORMAT_R16_UINT,
        0);

    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    context->IASetInputLayout(m_pInputLayout.Get());

    // Calling Draw tells Direct3D to start sending commands to the graphics device.
    context->DrawIndexed(
        (UINT)(m_indices.size()),
        0,
        0);
}
