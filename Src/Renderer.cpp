#include <cmath>

#include "Renderer.h"
#include "ShaderStructs.h"

Renderer::Renderer(
    std::shared_ptr<DeviceResources> deviceResources,
    std::shared_ptr<ShaderManager> shaderManager)
{
    m_deviceResources = deviceResources;
    m_shaderManager = shaderManager;
    m_frameCount = 0;
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

void Renderer::CreateWindowSizeDependentResources()
{
    CreateViewAndPerspective();
}

void Renderer::SetupScene()
{

    Model *plane = new Model(
        "Plane",
        "Res/Plane.arccmdl",
        m_deviceResources);
    Transform &tPlane = plane->GetTransform();
    tPlane.SetTranslation({0.0f, -1.5f, 0.0f});

    Model *cornellBox = new Model(
        "CornellBox",
        "Res/CornellBox.arccmdl",
        m_deviceResources);

    Transform &tCornellBox = cornellBox->GetTransform();
    tCornellBox.SetTranslation({0.0f, 0.0f, -1.75f});
    tCornellBox.SetRotation(
        {
            DirectX::XMConvertToRadians(94.0f),
            DirectX::XMConvertToRadians(63.0f),
            DirectX::XMConvertToRadians(37.0f),
        });

    Model *ball = new Model(
        "Ball",
        "Res/Ball.arccmdl",
        m_deviceResources);
    Transform &tBall = ball->GetTransform();
    tBall.SetTranslation({0.2, -0.1f, 0.0f});
    tBall.SetRotation(
        {
            DirectX::XMConvertToRadians(0.0f),
            DirectX::XMConvertToRadians(120.0f),
            DirectX::XMConvertToRadians(20.0f),
        });

    Model *monkey = new Model(
        "Monkey",
        "Res/Monkey.arccmdl",
        m_deviceResources);
    Transform &tMonkey = monkey->GetTransform();
    tMonkey.SetTranslation({2.25, 0.0f, -3.0f});
    tMonkey.SetRotation(
        {
            DirectX::XMConvertToRadians(10.0f),
            DirectX::XMConvertToRadians(-30.0f),
            DirectX::XMConvertToRadians(0.0f),
        });

    m_models.push_back(plane);
    m_models.push_back(cornellBox);
    m_models.push_back(ball);
    m_models.push_back(monkey);
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

void Renderer::Render()
{
    for (Model *&model : m_models)
        model->Render(
            m_constantBufferData.viewproj,
            m_shaderManager->GetCurrentShader());
}

Renderer::~Renderer()
{
    for (Model *&model : m_models)
        delete model;
}
