#pragma once
#include <vector>

#include "glad/glad.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Texture.h"
#include "Shader.h"
class FBO
{
public:
	unsigned int quadVAO, quadVBO;
	float width, height;

	GLuint ID;
	GLuint rboID;
	DepthTexture* depthTexture;
	std::vector<ColorTexture*> colorTextures = std::vector<ColorTexture*>(4, nullptr);
	Shader* shader;


	FBO(Shader* shader);
	~FBO();

	void update(float width, float height);

	void exportTexture(const char* fileName);

	void use();
	void unUse();
	void draw();
};

