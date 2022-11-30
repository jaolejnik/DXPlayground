#include "CubemapManager.h"

CubemapManager::CubemapManager(ID3D11Device *device, const std::vector<std::string> &cubemapNames)
{
    for (const std::string name : cubemapNames)
        m_cubemaps.emplace_back(new Cubemap(device, "Res/Cubemaps/" + name + "/"));
}

CubemapManager::~CubemapManager()
{
    for (Cubemap *&cubemap : m_cubemaps)
        delete cubemap;
}
