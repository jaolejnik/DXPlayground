#include <cmath>

#include "Renderer.h"
#include "ShaderStructs.h"
#include "Sh.h"

Renderer::Renderer(
    std::shared_ptr<DeviceResources> deviceResources,
    std::shared_ptr<ShaderManager> shaderManager,
    std::shared_ptr<CubemapManager> cubemapManager)
{
    m_deviceResources = deviceResources;
    m_shaderManager = shaderManager;
    m_pCubemapManager = cubemapManager;
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

    auto device = m_deviceResources->GetDevice();

    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

    device->CreateSamplerState(
        &sampDesc,
        &m_pSamplerState);
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
    for (Cubemap *&cubemap : m_pCubemapManager->GetCubemaps())
        m_sh->ProjectCubemapToSH(cubemap);
    // m_sh->ProjectToSH(LightPolar);
}

void Renderer::SetupScene()
{
    SetupModels();

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
    auto cubemap = m_pCubemapManager->GetCurrentCubemap();
    auto shCoeffs = cubemap->GetEncodedSHData();

    for (int i = 0; i < min(SHcoeffCount, 16); i++)
    {
        DirectX::XMStoreFloat4(
            m_sceneBufferData.shCoefficients + i,
            (*shCoeffs)[i]);
    }

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
            cubemap->GetResourceView(),
            m_pSamplerState.Get(),
            rasterState,
            shader);
    }
}

Renderer::~Renderer()
{
    delete m_camera;
    delete m_sh;

    for (Model *&model : m_models)
        delete model;
}
