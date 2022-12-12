#include "Light.h"

void PointLight::setup(Shader* shader)
{
	shader->setVec3("pointLight.position", position);
	shader->setVec3("pointLight.ambient", ambient);
	shader->setVec3("pointLight.diffuse", diffuse);
	shader->setVec3("pointLight.specular", specular);
	shader->setFloat("pointLight.constant", constant);
	shader->setFloat("pointLight.linear", linear);
	shader->setFloat("pointLight.quadratic", quadratic);
}

void SpotLight::setup(Shader* shader)
{
	shader->setVec3("spotLight.position", position);
	shader->setVec3("spotLight.direction", direction);
	shader->setVec3("spotLight.ambient", ambient);
	shader->setVec3("spotLight.diffuse", diffuse);
	shader->setVec3("spotLight.specular", specular);
	shader->setFloat("spotLight.constant", constant);
	shader->setFloat("spotLight.linear", linear);
	shader->setFloat("spotLight.quadratic", quadratic);
	shader->setFloat("spotLight.cutOff", cutOff);
	shader->setFloat("spotLight.outerCutOff", outerCutOff);
}

void DirLight::setup(Shader* shader)
{
	shader->setVec3("dirLight.direction", direction);
	shader->setVec3("dirLight.ambient", ambient);
	shader->setVec3("dirLight.diffuse", diffuse);
	shader->setVec3("dirLight.specular", specular);
}
