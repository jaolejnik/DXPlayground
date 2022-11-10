#pragma once

#include "DeviceResources.h"
#include "ShaderStructs.h"
#include "Transform.h"

class Model
{
private:
    const std::string m_name;
    Transform m_transform;
    TransformBufferStruct m_transformBufferData;
    std::vector<VertexPNC> m_vertices;
    std::vector<uint16_t> m_indices;

    // Direct3D device resources
    std::shared_ptr<DeviceResources> m_deviceResources;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pTransformBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pSceneBuffer;

    bool CreateFromFile(const std::string &filePath);
    HRESULT InitializeBuffers();

public:
    Model(
        const std::string name,
        const std::string &filePath,
        std::shared_ptr<DeviceResources> deviceResources);
    ~Model() {}

    void Animate(UINT frameCount);
    void Update(UINT frameCount);
    void Render(
        TransformBufferStruct *transformBufferData,
        SceneBufferStruct *sceneBufferData,
        ShaderStruct *shader);

    const char *GetName() { return m_name.c_str(); }
    Transform &GetTransform() { return m_transform; }
};
