#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../External/stb_image/stb_image.h"

Texture::Texture(const char *filePath)
{
	bool test = LoadTexture(filePath);
}

bool Texture::LoadTexture(const char *filePath)
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

DirectX::XMVECTOR Texture::SampleTexture(float u, float v)
{
	int x = u * static_cast<float>(width);
	int y = v * static_cast<float>(height);

	y *= 4;
	// TODO SOMETHING WRONG

	return {
		static_cast<float>(data[width * x + y + 0]) / 255.0f,
		static_cast<float>(data[width * x + y + 1]) / 255.0f,
		static_cast<float>(data[width * x + y + 2]) / 255.0f,
		static_cast<float>(data[width * x + y + 3]) / 255.0f,
	};
}

Texture::~Texture()
{
	free(data);
}
