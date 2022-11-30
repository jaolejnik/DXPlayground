#include "Cubemap.h"

Cubemap::Cubemap(ID3D11Device *device, const std::string dirPath)
{
    LoadCubemap(device, dirPath);
}

bool Cubemap::LoadCubemap(ID3D11Device *device, const std::string dirPath)
{
    HRESULT hr = S_OK;

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
        m_textureFaces.emplace_back(new Texture(fullPath, Faces(i)));
    }

    Texture *img = m_textureFaces.front();

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
        imgData[i].pSysMem = m_textureFaces[i]->data;
        imgData[i].SysMemPitch = m_textureFaces[i]->pitch;
        imgData[i].SysMemSlicePitch = 0;
    }

    ID3D11Texture2D *pTexture;
    hr = device->CreateTexture2D(&texDesc, imgData, &pTexture);

    D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
    ZeroMemory(&viewDesc, sizeof(viewDesc));
    viewDesc.Format = texDesc.Format;
    viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    viewDesc.TextureCube.MipLevels = texDesc.MipLevels;
    viewDesc.TextureCube.MostDetailedMip = 0;

    hr = device->CreateShaderResourceView(
        pTexture,
        &viewDesc,
        &m_pShaderResourceView);

    return true;
}

DirectX::XMVECTOR Cubemap::SampleCubemap(DirectX::XMFLOAT3 vec)
{
    Faces face;
    float u;
    float v;
    float maxAxis;

    bool isXPositive = vec.x > 0;
    bool isYPositive = vec.y > 0;
    bool isZPositive = vec.z > 0;

    float absX = std::abs(vec.x);
    float absY = std::abs(vec.y);
    float absZ = std::abs(vec.z);

    if (absX >= absY &&
        absX >= absZ)
    {
        maxAxis = absX;
        v = vec.y;

        if (isXPositive)
        {
            u = -vec.z;
            face = Faces::PosX;
        }
        else
        {
            u = vec.z;
            face = Faces::NegX;
        }
    }

    if (absY >= absX &&
        absY >= absZ)
    {
        maxAxis = absY;
        u = vec.x;

        if (isYPositive)
        {
            v = -vec.z;
            face = Faces::PosY;
        }
        else
        {
            v = vec.z;
            face = Faces::NegY;
        }
    }

    if (absZ >= absX &&
        absZ >= absY)
    {
        maxAxis = absZ;
        v = vec.y;

        if (isZPositive)
        {
            u = vec.x;
            face = Faces::PosZ;
        }
        else
        {
            u = -vec.x;
            face = Faces::NegZ;
        }
    }

    u = 0.5f * (u / maxAxis + 1.0f);
    v = 0.5f * (v / maxAxis + 1.0f);

    assert(face == m_textureFaces[static_cast<int>(face)]->face);

    return m_textureFaces[static_cast<int>(face)]->SampleTexture(u, v);
}

Cubemap::~Cubemap()
{
    for (Texture *&tf : m_textureFaces)
        delete tf;
}
