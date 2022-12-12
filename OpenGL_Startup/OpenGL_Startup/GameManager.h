#pragma once
#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "glad/glad.h"
#include "KHR/khrplatform.h"

#include "Renderer.h"
#include "SceneStack.h"
#include "SimpleScene.h"

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

void frame_buffer_size_callback_handle(GLFWwindow* window, int width, int height);
void error_callback(int error, const char* description);
void mouse_callback_handle(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback_handle(GLFWwindow* window, double xoffset, double yoffset);
void key_callback_handle(GLFWwindow* window, int key, int scancode, int action, int mods);

class GameManager
{
public:
	GameManager();
	~GameManager();
	void run();

private:

	std::shared_ptr<Renderer> m_Renderer;
	std::shared_ptr<SceneStack> m_SceneStack;
	GLFWwindow* m_Window;

	float deltaTime = 0.0f;	// time between current frame and last frame
	float lastFrame = 0.0f;

	float windowWidth, windowHeight;

	void processInput(GLFWwindow*);
	void update(float deltaTime);
	void render();

	void frame_buffer_size_callback(GLFWwindow* window, int width, int height);
	void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	void registerScenes();

	friend void frame_buffer_size_callback_handle(GLFWwindow* window, int width, int height);
	friend void mouse_callback_handle(GLFWwindow* window, double xposIn, double yposIn);
	friend void scroll_callback_handle(GLFWwindow* window, double xoffset, double yoffset);
	friend void key_callback_handle(GLFWwindow* window, int key, int scancode, int action, int mods);
};


