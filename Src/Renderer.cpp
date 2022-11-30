#include <cmath>

#include "Renderer.h"
#include "ShaderStructs.h"
#include "Sh.h"

Renderer::Renderer(
    std::shared_ptr<DeviceResources> deviceResources,
    std::shared_ptr<ShaderManager> shaderManager)
{
    m_deviceResources = deviceResources;
    m_shaderManager = shaderManager;
    m_camera = new Camera(
        {0.0f, 0.7f, 1.5f},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f});
    m_frameCount = 0;
}

void Renderer::CreateViewAndPerspective()
{
    float aspectRatioX = m_deviceResources->GetAspectRatio();
    DirectX::XMMATRIX viewproj = m_camera->GetViewProjectionMatrix(aspectRatioX);

    DirectX::XMStoreFloat4x4(
        &m_transformBufferData.viewproj,
        DirectX::XMMatrixTranspose(viewproj));
}

void Renderer::LoadCubeMap(const std::string &dirPath)
{
    HRESULT hr = S_OK;
    auto device = m_deviceResources->GetDevice();

    std::string faces[] = {
        "posx.jpg",
        "negx.jpg",
        "posy.jpg",
        "negy.jpg",
        "posz.jpg",
        "negz.jpg",
    };

    for (int i = 0; i < 6; i++)
    {
        std::string fullPath = dirPath + faces[i];
        m_loadedImages.emplace_back(new Texture(fullPath, Faces(i)));
    }

    Texture *img = m_loadedImages.front();

    D3D11_TEXTURE2D_DESC texDesc;
    ZeroMemory(&texDesc, sizeof(texDesc));
    texDesc.Width = img->width;
    texDesc.Height = img->height;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 6;
    texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texDesc.CPUAccessFlags = 0;
    texDesc.SampleDesc.Count = 1;
    texDesc.SampleDesc.Quality = 0;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = 0;
    texDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

    D3D11_SUBRESOURCE_DATA imgData[6];
    for (int i = 0; i < 6; i++)
    {
        imgData[i].pSysMem = m_loadedImages[i]->data;
        imgData[i].SysMemPitch = m_loadedImages[i]->pitch;
        imgData[i].SysMemSlicePitch = 0;
    }

    hr = device->CreateTexture2D(&texDesc, imgData, &m_pTexture);

    D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
    ZeroMemory(&viewDesc, sizeof(viewDesc));
    viewDesc.Format = texDesc.Format;
    viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    viewDesc.TextureCube.MipLevels = texDesc.MipLevels;
    viewDesc.TextureCube.MostDetailedMip = 0;

    hr = device->CreateShaderResourceView(
        m_pTexture.Get(),
        &viewDesc,
        &m_pShaderResourceView);

    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

    hr = device->CreateSamplerState(
        &sampDesc,
        &m_pSamplerState);
}

void Renderer::CreateWindowSizeDependentResources()
{
    LoadCubeMap("Res/Cubemaps/debug1/");
}

void Renderer::SetupModels()
{

    Model *skybox = new Model(
        "Skybox",
        "",
        m_deviceResources);

    Transform &tSkybox = skybox->GetTransform();
    tSkybox.SetScaleUniform(50.0f);

    Model *teapot = new Model(
        "Teapot",
        "Res/Models/Teapot.arccmdl",
        m_deviceResources);

    Transform &tTeapot = teapot->GetTransform();
    tTeapot.SetTranslation({0.1, -1.4f, 0.0f});
    tTeapot.SetRotation(
        {
            DirectX::XMConvertToRadians(90.0f),
            DirectX::XMConvertToRadians(170.0f),
            DirectX::XMConvertToRadians(0.0f),
        });
    tTeapot.SetScaleUniform(0.7f);

    Model *ball = new Model(
        "Ball",
        "Res/Models/Ball.arccmdl",
        m_deviceResources);
    Transform &tBall = ball->GetTransform();
    tBall.SetTranslation({-2.25, 0.0f, -3.0f});
    tBall.SetRotation(
        {
            DirectX::XMConvertToRadians(0.0f),
            DirectX::XMConvertToRadians(120.0f),
            DirectX::XMConvertToRadians(20.0f),
        });
    tBall.SetScaleUniform(4.0f);

    Model *monkey = new Model(
        "Monkey",
        "Res/Models/Monkey.arccmdl",
        m_deviceResources);
    Transform &tMonkey = monkey->GetTransform();
    tMonkey.SetTranslation({2.25, 0.0f, -3.0f});
    tMonkey.SetRotation(
        {
            DirectX::XMConvertToRadians(10.0f),
            DirectX::XMConvertToRadians(120.0f),
            DirectX::XMConvertToRadians(0.0f),
        });
    tMonkey.SetScaleUniform(1.3f);

    m_models.push_back(skybox);
    m_models.push_back(teapot);
    m_models.push_back(ball);
    m_models.push_back(monkey);
}

void Renderer::SetupSH()
{
    m_sh = new SphericalHarmonics(100, SHbands);
    m_sh->ProjectCubemapToSH(m_loadedImages);
    // m_sh->ProjectToSH(LightPolar);
}

void Renderer::SetupScene()
{
    SetupModels();
    SetupSH();

    auto shCoeffs = m_sh->GetEncodedCoefficients();
    for (int i = 0; i < min(SHcoeffCount, 16); i++)
    {
        DirectX::XMStoreFloat4(
            m_sceneBufferData.shCoefficients + i,
            shCoeffs[i]);
    }

    DirectX::XMFLOAT3 camPos = m_camera->GetPosition();
    m_sceneBufferData.lightPosition = {camPos.x, camPos.y, camPos.y, 1.0};
}

// Update the scene.
void Renderer::Tick()
{
    m_frameCount++;

    m_camera->Orbit(m_frameCount, 10.0f, {0.0f, 0.0f, 0.0f});

    for (Model *&model : m_models)
        model->Update(m_frameCount);

    if (m_frameCount == MAXUINT)
        m_frameCount = 0;
}

void Renderer::Render()
{
    CreateViewAndPerspective();

    for (Model *&model : m_models)
    {
        ShaderStruct *shader;
        ID3D11RasterizerState *rasterState;

        if (model->GetName() == "Skybox")
        {
            shader = m_shaderManager->GetShader(ShaderType::Skybox);
            rasterState = m_deviceResources->GetCullFrontState();
        }
        else
        {
            shader = m_shaderManager->GetCurrentShader();
            rasterState = m_deviceResources->GetCullBackState();
        }

        model->Render(
            &m_transformBufferData,
            &m_sceneBufferData,
            m_pShaderResourceView.Get(),
            m_pSamplerState.Get(),
            rasterState,
            shader);
    }
}

Renderer::~Renderer()
{
    delete m_camera;
    delete m_sh;

    for (Texture *&li : m_loadedImages)
        delete li;

    for (Model *&model : m_models)
        delete model;
}
