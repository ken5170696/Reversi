#include "GameObject.h"

GameObject::GameObject()
{
	m_model = nullptr;
}

GameObject::~GameObject()
{
	delete m_model;
}

GameObject::GameObject(const string& modelPath, glm::vec3 position)
{
	m_model = new Model(modelPath);
	m_transform.position = position;
}

GameObject::GameObject(const GameObject& obj)
{
	m_model = new Model(obj.m_model->directory);
	m_transform.position = obj.m_transform.position;
}

void GameObject::draw(Shader* shader)
{
	glm::mat4 modelMatrix = m_transform.getModelMatrix();
	shader->setMat4("model", modelMatrix);
	if (m_model) m_model->Draw(*shader);
}
