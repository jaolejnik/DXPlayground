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
    bool LoadTexture(const std::string filePath);

public:
    Faces face;
    int width;
    int height;
    int channels;
    int desiredChannels = 4;
    int pitch;
    unsigned char *data;

    Texture(const std::string filePath, Faces inFace);
    ~Texture();

    DirectX::XMVECTOR SampleTexture(float u, float v);
};
