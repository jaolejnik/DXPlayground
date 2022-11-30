#pragma once

#include "Cubemap.h"

constexpr char *CubemapNamesCString[]{
    "Yokohama",
    "Skansen",
    "Tenerife",
    "Fort",
    "debug1",
};

class CubemapManager
{
private:
    std::vector<Cubemap *> m_cubemaps;
    int m_currentCubemap = 0;

public:
    CubemapManager(ID3D11Device *device, const std::vector<std::string> &cubemapNames);
    ~CubemapManager();

    Cubemap *GetCurrentCubemap() { return m_cubemaps[m_currentCubemap]; }
    Cubemap *GetCubemap(int i) { return m_cubemaps[i]; }
    std::vector<Cubemap *> &GetCubemaps() { return m_cubemaps; }

    void SetCurrentCubemap(int i) { m_currentCubemap = i; }
};
