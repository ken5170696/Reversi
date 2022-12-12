#pragma once
#include "Shader.h"
#include "UBO.h"

class Renderer
{
public:
	Renderer();
	void Draw();

	/* Shader Section */
	std::shared_ptr<Shader> m_modelShader;
	std::shared_ptr<Shader> m_screenShader;

	std::shared_ptr<UBO> ubo;
};

