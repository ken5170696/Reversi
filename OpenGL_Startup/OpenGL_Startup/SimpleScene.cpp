#include "SimpleScene.h"

SimpleScene::SimpleScene(SceneStack _stack, SceneContext _context)
	: Scene(_stack, _context), m_context(_context)
{
	m_window = _context.window;
	glfwGetWindowSize(m_window, &vw, &vh);

	modelShader = _context.renderer->m_modelShader.get();

	m_board = std::make_shared<Model>("./resource/model/board/board.obj");
	m_camera = std::make_shared<Camera>();
	m_pointLight = std::make_shared<PointLight>();
	m_ubo = _context.renderer->ubo;
	isPause = false;
}

void SimpleScene::draw()
{
	modelShader->use();

	m_pointLight->setup(modelShader);
	glm::vec3 viewPos = m_camera->Position;
	modelShader->setVec3("viewPos", viewPos);
	modelShader->setMat4("model", glm::mat4(1));

	m_board->Draw(*modelShader);

	modelShader->unUse();
}

bool SimpleScene::update(float deltaTime)
{
	dt = deltaTime;

	modelShader->use();

	UBO::UpdateContent uc{ m_camera->GetProjectionMatrix(vw,vh), m_camera->GetViewMatrix() };
	m_ubo->update(uc);

	modelShader->unUse();

	return false;
}

bool SimpleScene::processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		m_camera->ProcessKeyboard(FORWARD, dt);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		m_camera->ProcessKeyboard(BACKWARD, dt);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		m_camera->ProcessKeyboard(LEFT, dt);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		m_camera->ProcessKeyboard(RIGHT, dt);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		m_camera->ProcessKeyboard(UP, dt);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		m_camera->ProcessKeyboard(DOWN, dt);

	return false;
}

void SimpleScene::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	if (!isPause)
		m_camera->ProcessMouseMovement(xoffset, yoffset);
}

void SimpleScene::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (!isPause)
		m_camera->ProcessMouseScroll(static_cast<float>(yoffset));
}

void SimpleScene::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
		if (isPause) {
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			isPause = false;
		}
		else {
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			isPause = true;
		}
	}
}
