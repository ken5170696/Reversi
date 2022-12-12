#include "GameManager.h"

GameManager::GameManager()
{
	// glfw: initialize and configure
	// ------------------------------
	if (!glfwInit()) {
		std::cerr << "glfwInit failed.\n";
		exit(-1);
	}
	glfwInitHint(GLFW_VERSION_MAJOR, 3);
	glfwInitHint(GLFW_VERSION_MINOR, 3);
	glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	m_Window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Project", nullptr, nullptr);
	if (m_Window == nullptr) {
		std::cerr << "Window Initialize failed.\n";
		exit(-1);
	}
	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwMakeContextCurrent(m_Window);

	glfwSetWindowUserPointer(m_Window, this);
	glfwSetFramebufferSizeCallback(m_Window, frame_buffer_size_callback_handle);
	glfwSetErrorCallback(error_callback); // tell GLFW to call error_callback if an internal error ever occur at some point inside GLFW functions.
	glfwSetCursorPosCallback(m_Window, mouse_callback_handle);
	glfwSetScrollCallback(m_Window, scroll_callback_handle);
	glfwSetKeyCallback(m_Window, key_callback_handle);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "gladLoadGLLoader failed.\n";
		exit(-1);
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.DisplaySize = ImVec2(100, 100);
	io.ConfigFlags |= ImGuiWindowFlags_NoBackground;
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
	const char* glsl_version = "#version 330";
	ImGui_ImplOpenGL3_Init(glsl_version);

	std::cout << "[Debug] Prepare Renderer\n";
	m_Renderer = (std::make_shared<Renderer>());

	std::cout << "[Debug] Prepare Scene Stack\n";
	m_SceneStack = (std::make_shared<SceneStack>(SceneContext(m_Window, m_Renderer)));
	registerScenes();
	m_SceneStack->pushState(SceneID::ID::Game);

	std::cout << "[Debug] initialize down.\n\n";
}

GameManager::~GameManager()
{
	/* ImGUI Clean up*/
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
}
void GameManager::run()
{

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glEnable(GL_DEPTH_TEST);


	std::cout << "[Debug] Game started\n";

	while (!glfwWindowShouldClose(m_Window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		processInput(m_Window);
		update(deltaTime);
		render();

		// Render dear imgui into screen
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}
}

void GameManager::processInput(GLFWwindow* window) 
{
	m_SceneStack->processInput(window);
}

void GameManager::update(float deltaTime)
{
	m_SceneStack->update(deltaTime);
}

void GameManager::render()
{
	glad_glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_SceneStack->draw();
}

void frame_buffer_size_callback_handle(GLFWwindow* window, int width, int height)
{
	GameManager* ptr = (GameManager*)glfwGetWindowUserPointer(window);
	ptr->frame_buffer_size_callback(window, width, height);
}

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s (%d)\n", description, error);
}

void mouse_callback_handle(GLFWwindow* window, double xposIn, double yposIn)
{
	GameManager* ptr = (GameManager*)glfwGetWindowUserPointer(window);
	ptr->mouse_callback(window, xposIn, yposIn);
}

void scroll_callback_handle(GLFWwindow* window, double xoffset, double yoffset)
{
	GameManager* ptr = (GameManager*)glfwGetWindowUserPointer(window);
	ptr->scroll_callback(window, xoffset, yoffset);
}

void key_callback_handle(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	GameManager* ptr = (GameManager*)glfwGetWindowUserPointer(window);
	ptr->key_callback(window, key, scancode, action, mods);
}

void GameManager::frame_buffer_size_callback(GLFWwindow* window, int width, int height)
{
	windowWidth = width;
	windowHeight = height;
	glViewport(0, 0, width, height);
}

void GameManager::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	m_SceneStack->mouse_callback(window, xposIn, yposIn);
}

void GameManager::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	m_SceneStack->scroll_callback(window, xoffset, yoffset);
}

void GameManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	m_SceneStack->key_callback(window, key, scancode, action, mods);
}

void GameManager::registerScenes()
{
	m_SceneStack->registerScene<SimpleScene>(SceneID::ID::Game);
}
