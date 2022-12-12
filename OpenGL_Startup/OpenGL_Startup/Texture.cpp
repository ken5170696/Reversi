#include "Texture.h"
#include "stb-master/stb_image.h"
#include "stb-master/stb_image_write.h"

ColorTexture::ColorTexture(GLenum type, float width, float height)
{
	this->width = width;
	this->height = height;
	this->type = type;
	glGenTextures(1, &this->id);
	glBindTexture(GL_TEXTURE_2D, this->id);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

ColorTexture::ColorTexture(const char* fileName, GLenum type)
{
	this->type = type;

	unsigned char* image = stbi_load(fileName, &this->width, &this->height, &nrChannels, 0);

	glGenTextures(1, &this->id);
	glBindTexture(type, this->id);

	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (image)
	{
		glTexImage2D(type, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(type);
	}
	else
	{
		std::cout << "ERROR::TEXTURE::TEXTURE_LOADING_FAILED: " << fileName << "\n";
	}

	glActiveTexture(0);
	glBindTexture(type, 0);
	stbi_image_free(image);
}

ColorTexture::~ColorTexture()
{
	glDeleteTextures(1, &this->id);
}

void ColorTexture::bind(const GLint texture_unit)
{
	glActiveTexture(GL_TEXTURE0 + texture_unit);
	glBindTexture(this->type, this->id);
}

void ColorTexture::unbind()
{
	glActiveTexture(0);
	glBindTexture(this->type, 0);
}

void ColorTexture::loadFromFile(const char* fileName)
{
	if (this->id)
	{
		glDeleteTextures(1, &this->id);
	}

	unsigned char* image = stbi_load(fileName, &this->width, &this->height, &nrChannels, 0);

	glGenTextures(1, &this->id);
	glBindTexture(this->type, this->id);

	glTexParameteri(this->type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(this->type, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (image)
	{
		glTexImage2D(this->type, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(this->type);
	}
	else
	{
		std::cout << "ERROR::TEXTURE::LOADFROMFILE::TEXTURE_LOADING_FAILED: " << fileName << "\n";
	}

	glActiveTexture(0);
	glBindTexture(this->type, 0);
	stbi_image_free(image);
}


DepthTexture::DepthTexture(GLenum type, float width, float height)
{
	this->width = width;
	this->height = height;
	this->type = type;
	glGenTextures(1, &this->id);
	glBindTexture(type, this->id);

	glTexImage2D(type, 0, GL_DEPTH_COMPONENT32F, this->width, this->height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

DepthTexture::DepthTexture(const char* fileName, GLenum type)
{
	this->type = type;

	unsigned char* image = stbi_load(fileName, &this->width, &this->height, &nrChannels, 0);

	glGenTextures(1, &this->id);
	glBindTexture(type, this->id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, this->width, this->height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glGenerateMipmap(type);
	}
	else
	{
		std::cout << "ERROR::TEXTURE::TEXTURE_LOADING_FAILED: " << fileName << "\n";
	}

	glActiveTexture(0);
	glBindTexture(type, 0);
	stbi_image_free(image);
}

DepthTexture::~DepthTexture()
{
	glDeleteTextures(1, &this->id);
}

void DepthTexture::bind(const GLint texture_unit)
{
	glActiveTexture(GL_TEXTURE0 + texture_unit);
	glBindTexture(this->type, this->id);
}

void DepthTexture::unbind()
{
	glActiveTexture(0);
	glBindTexture(this->type, 0);
}

void DepthTexture::loadFromFile(const char* fileName)
{
	if (this->id)
	{
		glDeleteTextures(1, &this->id);
	}

	unsigned char* image = stbi_load(fileName, &this->width, &this->height, &nrChannels, 0);

	glGenTextures(1, &this->id);
	glBindTexture(this->type, this->id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, this->width, this->height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glGenerateMipmap(this->type);
	}
	else
	{
		std::cout << "ERROR::TEXTURE::LOADFROMFILE::TEXTURE_LOADING_FAILED: " << fileName << "\n";
	}

	glActiveTexture(0);
	glBindTexture(this->type, 0);
	stbi_image_free(image);
}
