#pragma once

#include "DeviceResources.h"
#include "Model.h"
#include "ShaderManager.h"
#include "Camera.h"

class Renderer
{
private:
    std::shared_ptr<DeviceResources> m_deviceResources;
    std::shared_ptr<ShaderManager> m_shaderManager;

    std::vector<Texture *> m_loadedImages;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pShaderResourceView;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSamplerState;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pTexture;

    TransformBufferStruct m_transformBufferData;
    SceneBufferStruct m_sceneBufferData;
    Camera *m_camera;
    std::vector<Model *> m_models;
    unsigned int m_frameCount;

    void CreateViewAndPerspective();
    void LoadCubeMap(const std::string &dirPath);

public:
    Renderer(
        std::shared_ptr<DeviceResources> deviceResources,
        std::shared_ptr<ShaderManager> shaderManager);
    ~Renderer();

    void CreateWindowSizeDependentResources();
    void SetupScene();
    void Tick();
    void Render();

    void SetCurrentShader(int i) { m_shaderManager->SetCurrentShader(i); }

    std::vector<Model *> &GetModels() { return m_models; }
};
