#pragma once
#include <memory>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "Renderer.h"

class SceneContext {
public:
	SceneContext(GLFWwindow* window, std::shared_ptr<Renderer> renderer)
	{
		this->window = window;
		this->renderer = renderer;
	};

	GLFWwindow* window;
	std::shared_ptr<Renderer> renderer;
};

namespace SceneID {
	enum ID
	{
		Game,
		None,
	};
};