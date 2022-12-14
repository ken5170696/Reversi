#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Scene.h"

#include "UBO.h"
#include "Model.h"
#include "Camera.h"
#include "Light.h"
#include "BoardManager.h"
#include "FBO.h"
#include "ShadowMap.h"
#include "Floor.h"
#include "Speaker.h"

class SimpleSceneGUI {
public:
	const string str = {
		"    A B C D E F G F  \n"
		" 	 _ _ _ _ _ _ _ _  \n"
		"1	|_|_|_|_|_|_|_|_| \n"
		"2	|_|_|_|_|_|_|_|_| \n"
		"3	|_|_|_|_|_|_|_|_| \n"
		"4	|_|_|_|_|_|_|_|_| \n"
		"5	|_|_|_|_|_|_|_|_| \n"
		"6	|_|_|_|_|_|_|_|_| \n"
		"7	|_|_|_|_|_|_|_|_| \n"
		"8	|_|_|_|_|_|_|_|_| \n"
	};
	float musicVolumn = 0.5;
	bool gameStartBtn = false;
	bool gameStarting = false;
	bool WhiteBot = false;
	bool BlackBot = false;
	string gameStateStr = "";
	string winner = "";
	float botSpeed = 1.5;

	bool ShadowDebug = false;
	bool ShadowOn = true;
	float dirLightPosition[3] = { 0.0f, -19.0f, 0.0f };
	float dirLightDirection[3] = { -1.0f, -5.0f, -2.0f };

	float maxBias = 0.015, minBias = 0.0001;
	int sampleNum = 3;

	void GameUI();
};

class SimpleScene : public Scene
{
public:
	SimpleScene( SceneStack _scene, SceneContext _context);

	virtual void draw() override;
	virtual bool update(float deltaTime) override;
	virtual bool processInput(GLFWwindow* window) override;

	virtual void frame_buffer_size_callback(GLFWwindow* window, int width, int height) override;
	virtual void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) override;
	virtual void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) override;
	virtual void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) override;
	virtual void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) override;

	FBO screenFBO;
	FBO edgeFBO;

	std::shared_ptr<Camera> m_camera;
	std::shared_ptr<DirLight> m_dirLight;
	std::shared_ptr<PointLight> m_pointLight;
	std::shared_ptr<UBO> m_ubo;

	ShadowMap shadowMap;

	BoardManager* boardManager;
	Board m_board;

	Floor floor;
	Speaker speaker;

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
	Shader* edgeShader;


	SimpleSceneGUI ui;
};

