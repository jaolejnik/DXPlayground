#pragma once

#include <string>

#include "../pch.h"

enum class Faces
{
    PosX,
    NegX,
    PosY,
    NegY,
    PosZ,
    NegZ
};

class Texture
{
private:
    bool LoadTexture(const char *filePath);

public:
    int width;
    int height;
    int channels;
    int desiredChannels = 4;
    int pitch;
    unsigned char *data;

    Texture(const char *filePath);
    ~Texture();

    DirectX::XMVECTOR SampleTexture(float u, float v);
};
