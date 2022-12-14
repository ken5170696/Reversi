#pragma once
#include <vector>

#include "glad/glad.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Texture.h"
#include "Shader.h"
class ShadowMap
{
public:
	unsigned int quadVAO, quadVBO;
	float width, height;

	GLuint ID;
	DepthTexture* depthTexture;
	std::shared_ptr<Shader> shader;
	std::shared_ptr<Shader> debugShader;

	ShadowMap( std::shared_ptr<Shader> shader, std::shared_ptr<Shader> debugShader);
	~ShadowMap();

	void update(float width, float height);
	glm::mat4 lightSpaceMatrix(glm::vec2 range, glm::vec3 position, glm::vec3 direction) const;

	void use();
	void unUse();
	void draw();
	void exportTexture(const char* fileName);
	void renderQuad();
};

