#pragma once

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include <iostream>;
#include "Transform.h"
#include "Model.h"
#include "Shader.h"

#define BLACK_CHESS_MODEL_PATH "./resource/model/black_chess/black_chess.obj"
#define WHITW_CHESS_MODEL_PATH "./resource/model/white_chess/white_chess.obj"

class GameObject
{
public:

	GameObject();
	virtual ~GameObject();
	GameObject(const string& modelPath, glm::vec3 position);
	GameObject(const GameObject& obj);

	virtual void draw(Shader* shader);

	Transform m_transform;

protected:
	Model* m_model;
};