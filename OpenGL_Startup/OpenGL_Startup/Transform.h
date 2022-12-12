#pragma once
#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform
{
public:
	glm::vec3 position;
	glm::vec3 rotation; // degree
	glm::vec3 scale;

	Transform() {
		position = glm::vec3(0.0, 0.0, 0.0);
		rotation = glm::vec3(0.0, 0.0, 0.0);
		scale = glm::vec3(1.0, 1.0, 1.0);
	}

	glm::mat4 getModelMatrix() {
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, position);

		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0, 0.0, 0.0));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0, 1.0, 0.0));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0, 0.0, 1.0));

		modelMatrix = glm::scale(modelMatrix, scale);
		return modelMatrix;
	};
};

