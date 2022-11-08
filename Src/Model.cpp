#include <fstream>

#include "Model.h"

Model::Model(
    const std::string name,
    const std::string &filePath,
    std::shared_ptr<DeviceResources> deviceResources)
    : m_name(name)
{
    m_deviceResources = deviceResources;
    CreateFromFile(filePath);
    InitializeBuffers();
}

bool Model::CreateFromFile(const std::string &filePath)
{
    int n;
    std::ifstream modelFile(filePath);

    if (!modelFile.is_open())
        return false;

    modelFile >> n;
    for (int i = 0; i < n; i += 9)
    {
        VertexPNC vpnc;

        modelFile >> vpnc.position.x;
        modelFile >> vpnc.position.y;
        modelFile >> vpnc.position.z;

        // TODO find out why normal and color memory are swapped
        modelFile >> vpnc.color.x;
        modelFile >> vpnc.color.y;
        modelFile >> vpnc.color.z;

        modelFile >> vpnc.normal.x;
        modelFile >> vpnc.normal.y;
        modelFile >> vpnc.normal.z;

        m_vertices.push_back(vpnc);
    }

    modelFile >> n;
    for (int i = 0; i < n; i++)
    {
        uint16_t d;
        modelFile >> d;

        m_indices.push_back(d);
    }

    modelFile.close();

    return true;
}

HRESULT Model::InitializeBuffers()
{
    HRESULT hr = S_OK;

    // Use the Direct3D device to load resources into graphics memory.
    ID3D11Device *device = m_deviceResources->GetDevice();

    // Create vertex buffer:
    CD3D11_BUFFER_DESC vDesc(
        static_cast<UINT>(sizeof(VertexPNC) * m_vertices.size()),
        D3D11_BIND_VERTEX_BUFFER);

    D3D11_SUBRESOURCE_DATA vData = {m_vertices.data()};
    hr = device->CreateBuffer(
        &vDesc,
        &vData,
        &m_pVertexBuffer);

    // Create index buffer:
    CD3D11_BUFFER_DESC iDesc(
        static_cast<UINT>(sizeof(uint16_t) * m_indices.size()),
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

void Model::Render(
    DirectX::XMFLOAT4X4 viewproj,
    ShaderStruct *shader)

{
    // Use the Direct3D device context to draw.
    ID3D11DeviceContext *context = m_deviceResources->GetDeviceContext();

    // Set up the vertex shader stage.
    context->VSSetShader(
        shader->vertexShader,
        nullptr,
        0);

    // Set up the pixel shader stage.
    context->PSSetShader(
        shader->pixelShader,
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
    UINT stride = sizeof(VertexPNC);
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

    context->IASetInputLayout(shader->inputLayout);

    // Calling Draw tells Direct3D to start sending commands to the graphics device.
    context->DrawIndexed(
        static_cast<UINT>(m_indices.size()),
        0,
        0);
}
