#pragma once

#include "../pch.h"

#include "ShaderStructs.h"
#include "DeviceResources.h"

class ShaderManager
{
private:
    std::shared_ptr<DeviceResources> m_deviceResources;
    std::vector<ShaderStruct *> m_shaders;
    int m_currentShader = 0;

    void CreateInputLayout(const ShaderType &shaderType, std::vector<D3D11_INPUT_ELEMENT_DESC> &iaDesc);
    HRESULT CreateShader(const ShaderType &shaderType);

public:
    ShaderManager(
        std::shared_ptr<DeviceResources> deviceResources,
        const std::vector<ShaderType> &shaderTypes);
    ~ShaderManager();

    void SetCurrentShader(int i) { m_currentShader = i; }
    ShaderStruct *GetCurrentShader() { return m_shaders[m_currentShader]; }
};
