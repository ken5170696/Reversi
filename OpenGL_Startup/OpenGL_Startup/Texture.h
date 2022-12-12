#pragma once
/* reference: https://github.com/Headturna/OpenGL-C---Tutorials */

#include <iostream>
#include <string>
#include <fstream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "KHR/khrplatform.h"

class Texture
{
protected:
	GLuint id;
	int width;
	int height;
	int nrChannels;
	unsigned int type;

public:
	virtual inline GLuint getID() const { return this->id; }

	virtual void bind(const GLint texture_unit) = 0;

	virtual void unbind() = 0;

	virtual void loadFromFile(const char* fileName) = 0;
};

class ColorTexture : public Texture
{

public:

	ColorTexture(GLenum type, float width, float height);
	ColorTexture(const char* fileName, GLenum type);

	~ColorTexture();

	void bind(const GLint texture_unit);

	void unbind();

	void loadFromFile(const char* fileName);
};

class DepthTexture : public Texture
{

public:

	DepthTexture(GLenum type, float width, float height);
	DepthTexture(const char* fileName, GLenum type);

	~DepthTexture();

	void bind(const GLint texture_unit);

	void unbind();

	void loadFromFile(const char* fileName);
};