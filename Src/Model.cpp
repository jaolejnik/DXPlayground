#include <fstream>

#include "Model.h"

Model::Model(
    const std::string name,
    const std::string &filePath,
    std::shared_ptr<DeviceResources> deviceResources)
    : m_name(name)
{
    m_deviceResources = deviceResources;
    if (filePath == "")
        CreateSphere(1.0f, 100u, 100u);
    else
        CreateFromFile(filePath);
    InitializeBuffers();
}

void Model::CreateSphere(const float radius,
                         const uint16_t longitudeSplitCount,
                         const uint16_t latitudeSplitCount)
{
    const auto longitudeSplitEdgesCount = longitudeSplitCount + 1u;
    const auto latitudeSplitEdgesCount = latitudeSplitCount + 1u;
    const auto longitudeSplitVerticesCount = longitudeSplitEdgesCount + 1u;
    const auto latitudeSplitVerticesCount = latitudeSplitEdgesCount + 1u;
    const auto vertexCount = longitudeSplitVerticesCount * latitudeSplitVerticesCount;
    const auto indexCount = 3u * 2u * longitudeSplitEdgesCount * latitudeSplitEdgesCount;

    m_vertices = std::vector<VertexPNC>(vertexCount);
    m_indices = std::vector<uint16_t>(indexCount);

    const float dTheta = DirectX::XM_2PI / (static_cast<float>(longitudeSplitEdgesCount));
    const float dPhi = DirectX::XM_PI / (static_cast<float>(latitudeSplitEdgesCount));

    uint16_t index = 0u;
    float phi = 0.0f;
    for (uint16_t i = 0u; i < latitudeSplitVerticesCount; i++)
    {
        const float cos_phi = cos(phi);
        const float sin_phi = sin(phi);
        float theta = 0.0f;

        for (uint16_t j = 0u; j < longitudeSplitVerticesCount; j++)
        {
            const float cos_theta = cos(theta);
            const float sin_theta = sin(theta);

            m_vertices[index].position = {
                radius * sin_theta * sin_phi,
                -radius * cos_phi,
                radius * cos_theta * sin_phi,
            };

            m_vertices[index].color = {
                static_cast<float>(j) / static_cast<float>(longitudeSplitVerticesCount),
                static_cast<float>(i) / static_cast<float>(latitudeSplitVerticesCount),
                0.0f,
            };

            DirectX::XMVECTOR tangent = {cos_theta, 0.0f, -sin_theta};
            DirectX::XMVECTOR binormal = {sin_theta * cos_phi,
                                          sin_phi,
                                          cos_theta * cos_phi};

            DirectX::XMStoreFloat3(
                &m_vertices[index].normal,
                DirectX::XMVector3Cross(tangent, binormal));

            theta += dTheta;
            index++;
        }

        phi += dPhi;
    }

    index = 0u;
    for (uint16_t i = 0u; i < latitudeSplitEdgesCount; i++)
    {
        for (uint16_t j = 0u; j < longitudeSplitEdgesCount; j++)
        {

            m_indices[index + 2u] = longitudeSplitVerticesCount * (i + 0u) + (j + 0u);
            m_indices[index + 1u] = longitudeSplitVerticesCount * (i + 0u) + (j + 1u);
            m_indices[index + 0u] = longitudeSplitVerticesCount * (i + 1u) + (j + 1u);

            index += 3;

            m_indices[index + 2u] = longitudeSplitVerticesCount * (i + 0u) + (j + 0u);
            m_indices[index + 1u] = longitudeSplitVerticesCount * (i + 1u) + (j + 1u);
            m_indices[index + 0u] = longitudeSplitVerticesCount * (i + 1u) + (j + 0u);

            index += 3;
        }
    }
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

        modelFile >> vpnc.normal.x;
        modelFile >> vpnc.normal.y;
        modelFile >> vpnc.normal.z;

        modelFile >> vpnc.color.x;
        modelFile >> vpnc.color.y;
        modelFile >> vpnc.color.z;

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

    // Create constant buffers
    CD3D11_BUFFER_DESC cbtDesc(
        sizeof(TransformBufferStruct),
        D3D11_BIND_CONSTANT_BUFFER);

    hr = device->CreateBuffer(
        &cbtDesc,
        nullptr,
        &m_pTransformBuffer);

    CD3D11_BUFFER_DESC cbsDesc(
        sizeof(SceneBufferStruct),
        D3D11_BIND_CONSTANT_BUFFER);

    hr = device->CreateBuffer(
        &cbsDesc,
        nullptr,
        &m_pSceneBuffer);

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
    DirectX::XMStoreFloat4x4(
        &m_transformBufferData.model,
        DirectX::XMMatrixTranspose(m_transform.GetTransformMatrix()));
}

void Model::Render(
    TransformBufferStruct *transformBufferData,
    SceneBufferStruct *sceneBufferData,
    ID3D11ShaderResourceView *resourceView,
    ID3D11SamplerState *sampler,
    ID3D11RasterizerState *rasterState,
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

    context->PSSetShaderResources(
        0,
        1,
        &resourceView);

    context->PSSetSamplers(
        0,
        1,
        &sampler);

    context->VSSetConstantBuffers(
        0,
        1,
        m_pTransformBuffer.GetAddressOf());

    m_transformBufferData.viewproj = transformBufferData->viewproj;
    context->UpdateSubresource(
        m_pTransformBuffer.Get(),
        0,
        nullptr,
        &m_transformBufferData,
        0,
        0);

    context->VSSetConstantBuffers(
        1,
        1,
        m_pSceneBuffer.GetAddressOf());

    context->PSSetConstantBuffers(
        1,
        1,
        m_pSceneBuffer.GetAddressOf());

    context->UpdateSubresource(
        m_pSceneBuffer.Get(),
        0,
        nullptr,
        sceneBufferData,
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

    context->RSSetState(rasterState);

    // Calling Draw tells Direct3D to start sending commands to the graphics device.
    context->DrawIndexed(
        static_cast<UINT>(m_indices.size()),
        0,
        0);
}
