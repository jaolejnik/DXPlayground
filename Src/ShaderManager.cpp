#include "ShaderManager.h"

ShaderManager::ShaderManager(
    std::shared_ptr<DeviceResources> deviceResources,
    const std::vector<ShaderType> &shaderTypes)
{
    m_deviceResources = deviceResources;

    for (const ShaderType &st : shaderTypes)
        CreateShader(st);
}

void ShaderManager::CreateInputLayout(const ShaderType &shaderType, std::vector<D3D11_INPUT_ELEMENT_DESC> &iaDesc)
{
    switch (shaderType)
    {
    case ShaderType::Diffuse:
    case ShaderType::Normal:
        iaDesc = {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
             0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},

            {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,
             0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},

            {"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT,
             0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
        };
        break;
    case ShaderType::Skybox:
        iaDesc = {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
             0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},

            {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,
             0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},

            {"TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT,
             0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
        };
        break;

    default:
        break;
    }
}

HRESULT ShaderManager::CreateShader(const ShaderType &shaderType)
{
    HRESULT hr = S_OK;

    // Use the Direct3D device to load resources into graphics memory.
    ID3D11Device *device = m_deviceResources->GetDevice();

    const std::string stmp = std::string(ShaderTypeToCstring[static_cast<int>(shaderType)]);
    const std::wstring shaderName = std::wstring(stmp.begin(), stmp.end());
    std::wstring vertexShaderFile = shaderName + L"VertexShader.cso";
    std::wstring pixelShaderFile = shaderName + L"PixelShader.cso";

    ShaderStruct *shader = new ShaderStruct;

    ID3DBlob *vertexShaderBlob = NULL;
    hr = D3DReadFileToBlob(vertexShaderFile.c_str(), &vertexShaderBlob);

    ID3DBlob *pixelShaderBlob = NULL;
    hr = D3DReadFileToBlob(pixelShaderFile.c_str(), &pixelShaderBlob);

    device->CreateVertexShader(
        vertexShaderBlob->GetBufferPointer(),
        vertexShaderBlob->GetBufferSize(),
        nullptr,
        &shader->vertexShader);

    device->CreatePixelShader(
        pixelShaderBlob->GetBufferPointer(),
        pixelShaderBlob->GetBufferSize(),
        nullptr,
        &shader->pixelShader);

    std::vector<D3D11_INPUT_ELEMENT_DESC> iaDesc;
    CreateInputLayout(shaderType, iaDesc);

    hr = device->CreateInputLayout(
        iaDesc.data(),
        static_cast<UINT>(iaDesc.size()),
        vertexShaderBlob->GetBufferPointer(),
        vertexShaderBlob->GetBufferSize(),
        &shader->inputLayout);

    m_shaders.push_back(shader);

    return hr;
}

ShaderManager::~ShaderManager()
{
    for (ShaderStruct *&shader : m_shaders)
    {
        shader->vertexShader->Release();
        shader->pixelShader->Release();
        shader->inputLayout->Release();
        delete shader;
    }
}
