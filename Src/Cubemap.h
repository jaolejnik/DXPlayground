#pragma once

#include "Texture.h"

class Cubemap
{
private:
    std::string m_name;
    std::vector<Texture *> m_textureFaces;
    ID3D11ShaderResourceView *m_pShaderResourceView;

    bool LoadCubemap(ID3D11Device *device, const std::string dirPath);

public:
    std::vector<DirectX::XMVECTOR> m_encodedSHData;
    Cubemap(ID3D11Device *device, const std::string dirPath);
    ~Cubemap();

    DirectX::XMVECTOR SampleCubemap(DirectX::XMFLOAT3 vec);

    std::vector<DirectX::XMVECTOR> *GetEncodedSHData() { return &m_encodedSHData; }
    ID3D11ShaderResourceView *GetResourceView() { return m_pShaderResourceView; }
};
