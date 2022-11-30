#pragma once

#include "DeviceResources.h"
#include "Model.h"
#include "Camera.h"
#include "Sh.h"
#include "ShaderManager.h"
#include "CubemapManager.h"

class Renderer
{
private:
    std::shared_ptr<DeviceResources> m_deviceResources;
    std::shared_ptr<ShaderManager> m_shaderManager;
    std::shared_ptr<CubemapManager> m_pCubemapManager;

    Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSamplerState;
    TransformBufferStruct m_transformBufferData;
    SceneBufferStruct m_sceneBufferData;

    Camera *m_camera;
    SphericalHarmonics *m_sh;
    std::vector<Model *> m_models;
    unsigned int m_frameCount;

    void CreateViewAndPerspective();

public:
    Renderer(
        std::shared_ptr<DeviceResources> deviceResources,
        std::shared_ptr<ShaderManager> shaderManager,
        std::shared_ptr<CubemapManager> cubemapManager);
    ~Renderer();

    void SetupModels();
    void SetupSH();
    void SetupScene();
    void Tick();
    void Render();

    void SetCurrentShader(int i) { m_shaderManager->SetCurrentShader(i); }
    void SetCurrentCubemap(int i) { m_pCubemapManager->SetCurrentCubemap(i); }

    std::vector<Model *> &GetModels() { return m_models; }
};
