#pragma once

#include <string>

#include "../pch.h"

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
};
