#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../External/stb_image/stb_image.h"

Texture::Texture(const std::string filePath, Faces inFace)
{
	LoadTexture(filePath);
	face = inFace;
}

bool Texture::LoadTexture(const std::string filePath)
{

	data = stbi_load(
		filePath.c_str(),
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

	int index = (x + height * y) * 4;

	return {
		static_cast<float>(data[index + 0]) / 255.0f,
		static_cast<float>(data[index + 1]) / 255.0f,
		static_cast<float>(data[index + 2]) / 255.0f,
		static_cast<float>(data[index + 3]) / 255.0f,
	};
}

Texture::~Texture()
{
	free(data);
}
