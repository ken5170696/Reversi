#pragma once
#include "Shader.h"

class Light
{
	virtual void setup(Shader* shader) = 0;
};

class DirLight : public Light {

public:
	glm::vec3 direction = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 ambient = glm::vec3(0.3f, 0.3f, 0.3f);;
	glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);;
	glm::vec3 specular = glm::vec3(0.4f, 0.4f, 0.4f);;

	virtual void setup(Shader* shader) override;
};

class PointLight : public Light {

public:
	glm::vec3 position = glm::vec3(0.0f, 1.0f, 0.0f);

	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;

	glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	glm::vec3 diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
	glm::vec3 specular = glm::vec3(0.3f, 0.3f, 0.3f);

	virtual void setup(Shader* shader) override;
};

class SpotLight : public Light {

public:
	glm::vec3 position = glm::vec3(0.0f, 2.0f, 0.0f);
	glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);
	float cutOff = glm::cos(glm::radians(10.0f));
	float outerCutOff = glm::cos(glm::radians(12.5f));

	float constant = 1.0f;
	float linear = 0.009f;
	float quadratic = 0.0032f;

	glm::vec3 ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	/*glm::vec3 diffuse = glm::vec3(0.3f, 0.3f, 0.3f);
	glm::vec3 specular = glm::vec3(0.1f, 0.1f, 0.1f);*/
	glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 specular = glm::vec3(0.3f, 0.3f, 0.3f);

	virtual void setup(Shader* shader) override;
};
