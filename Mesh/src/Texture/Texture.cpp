#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad\glad.h>

Texture::Texture(const char * path)
{
	LoadFromFile(path);
}

Texture::~Texture(void)
{
	glDeleteTextures(1, &_texID);
}

bool Texture::LoadFromFile(const char * path)
{
	stbi_set_flip_vertically_on_load(1);

	int bytes;

	_data = stbi_load(path, &_width, &_height, &bytes, 4);

	if (!_data)
		return false;

	glEnable(GL_TEXTURE_2D);
	
	glGenTextures(1, &_texID);
	glBindTexture(GL_TEXTURE_2D, _texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _data);
	
	//stbi_image_free(_data);

	return true;
}

void Texture::Bind(int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, _texID);
}

void Texture::Unbind(int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture Texture::white;