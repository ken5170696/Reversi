#include "Renderer.h"

Renderer::Renderer()
{
	m_modelShader = std::make_shared<Shader>("./resource/shader/modelShader.vert", "./resource/shader/modelShader.frag");
	m_screenShader = std::make_shared<Shader>("./resource/shader/screenShader.vert", "./resource/shader/screenShader.frag");

	ubo = std::make_shared<UBO>();
	ubo->setup(m_modelShader);
}

void Renderer::Draw()
{

}
