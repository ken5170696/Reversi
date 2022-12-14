#include "Renderer.h"

Renderer::Renderer()
{
	m_modelShader = std::make_shared<Shader>("./resource/shader/modelShader.vert", "./resource/shader/modelShader.frag");
	m_screenShader = std::make_shared<Shader>("./resource/shader/screenShader.vert", "./resource/shader/screenShader.frag");
	m_boundingboxShader = std::make_shared<Shader>("./resource/shader/boundingbox.vert", "./resource/shader/boundingbox.frag");
	m_shadowShader = std::make_shared<Shader>("./resource/shader/shadow_mapping_depth.vert", "./resource/shader/shadow_mapping_depth.frag");
	m_shadowDebugShader = std::make_shared<Shader>("./resource/shader/debug_quad_depth.vert", "./resource/shader/debug_quad_depth.frag");
	m_edgeShader = std::make_shared<Shader>("./resource/shader/edgeShader.vert", "./resource/shader/edgeShader.frag");
	
	ubo = std::make_shared<UBO>();

	ubo->setup(m_modelShader);
	ubo->setup(m_boundingboxShader);
	ubo->setup(m_edgeShader);
}

void Renderer::Draw()
{

}
