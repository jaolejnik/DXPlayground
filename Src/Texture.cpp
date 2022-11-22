#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../External/stb_image/stb_image.h"

Texture::Texture(const char* filePath)
{
	bool test = LoadTexture(filePath);
}

bool Texture::LoadTexture(const char* filePath)
{

	data = stbi_load(
		filePath,
		&width,
		&height,
		&channels,
		desiredChannels);

	if (!data)
		return false;

	pitch = 4 * width;

	return true;
}

Texture::~Texture()
{
	free(data);
}
