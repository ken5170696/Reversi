#pragma once
#include "Scene.h"

#include "UBO.h"
#include "Model.h"
#include "Camera.h"
#include "Light.h"

class SimpleScene : public Scene
{
public:
	SimpleScene( SceneStack _scene, SceneContext _context);

	virtual void draw() override;
	virtual bool update(float deltaTime) override;
	virtual bool processInput(GLFWwindow* window) override;

	virtual void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) override;
	virtual void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) override;
	virtual void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) override;

private:
	std::shared_ptr<Model> m_board;
	std::shared_ptr<Camera> m_camera;
	std::shared_ptr<PointLight> m_pointLight;
	std::shared_ptr<UBO> m_ubo;

	int vh;
	int vw;
	float dt;

	bool isPause;

	float lastX = (float)vw / 2.0f;
	float lastY = (float)vh / 2.0f;
	bool firstMouse = true;

	GLFWwindow* m_window;
	SceneContext m_context;
	Shader* modelShader;
};

