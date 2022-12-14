#include "SimpleScene.h"

SimpleScene::SimpleScene(SceneStack _stack, SceneContext _context)
	: Scene(_stack, _context), m_context(_context), m_board()
	, screenFBO(_context.renderer->m_screenShader.get()),
	edgeFBO(_context.renderer->m_screenShader.get()),
	shadowMap(_context.renderer->m_shadowShader, _context.renderer->m_shadowDebugShader),
	floor(), speaker()
{

	std::cout << "[Debug] Simple Scene initialize.\n";

	m_window = _context.window;
	glfwGetWindowSize(m_window, &vw, &vh);

	modelShader = _context.renderer->m_modelShader.get();
	edgeShader = _context.renderer->m_edgeShader.get();

	boardManager = new BoardManager(_context.renderer.get());

	m_camera = std::make_shared<Camera>();
	m_pointLight = std::make_shared<PointLight>();
	m_dirLight = std::make_shared<DirLight>();
	m_ubo = _context.renderer->ubo;
	isPause = false;

	std::cout << "[Debug] Simple Scene initialize success.\n";
}

void SimpleScene::draw()
{
	/* "Record" to Screen FBO*/
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	shadowMap.use();
		shadowMap.shader->use();
			m_board.draw(shadowMap.shader.get());
			boardManager->draw(shadowMap.shader.get());
			floor.draw(shadowMap.shader.get());
			speaker.draw(shadowMap.shader.get());
		shadowMap.shader->unUse();
	shadowMap.unUse();

	glViewport(0,0,vw,vh);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	screenFBO.use();
		modelShader->use();
			shadowMap.depthTexture->bind(5);
			modelShader->setInt("shadowMap", 5);

			m_board.draw(modelShader);
			boardManager->draw(modelShader);
			floor.draw(modelShader);
			speaker.draw(modelShader);
		modelShader->unUse();
	screenFBO.unUse();

	/* edge glowing effect */
	edgeFBO.use();
		edgeShader->use();

		if(speaker.isHover) speaker.draw(edgeShader);

		Chess* tmpChess = boardManager->getHoverChess();
		if(tmpChess) tmpChess->draw(edgeShader);

		edgeShader->unUse();
	edgeFBO.unUse();

	/* Show Screen FBO*/
	screenFBO.shader->use();
	edgeFBO.colorTextures[0]->bind(7);
	screenFBO.shader->setInt("edgeTexture",7);
	screenFBO.draw();

	if (ui.ShadowDebug) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shadowMap.debugShader->use();
		shadowMap.debugShader->setFloat("near_plane", 1.0);
		shadowMap.debugShader->setFloat("far_plane", 100.0);
		shadowMap.renderQuad();
	}

	ui.GameUI();
	//ImGui::ShowDemoWindow();
}

bool SimpleScene::update(float deltaTime)
{
	dt = deltaTime;

	/* Shader */

	// Shadow Shader
	shadowMap.shader->use();
	shadowMap.shader->setMat4("lightSpaceMatrix",
		shadowMap.lightSpaceMatrix(glm::vec2(-20.0f, 20.0f), glm::vec3(ui.dirLightPosition[0], ui.dirLightPosition[1], ui.dirLightPosition[2]), m_dirLight->direction)
	);
	shadowMap.shader->unUse();

	// Model Shader
	modelShader->use();
	m_dirLight->setup(modelShader);
	m_pointLight->setup(modelShader);
	glm::vec3 viewPos = m_camera->Position;
	modelShader->setVec3("viewPos", viewPos);
	UBO::UpdateContent uc{ m_camera->GetProjectionMatrix(vw,vh), m_camera->GetViewMatrix() };
	m_ubo->update(uc);
	modelShader->setMat4("lightSpaceMatrix",
		shadowMap.lightSpaceMatrix(glm::vec2(-20.f, 20.0f), glm::vec3(ui.dirLightPosition[0], ui.dirLightPosition[1], ui.dirLightPosition[2]), m_dirLight->direction)
	);
	modelShader->setFloat("minBias", ui.minBias);
	modelShader->setFloat("maxBias", ui.maxBias);
	modelShader->setInt("sampleNum", ui.sampleNum);
	modelShader->setBool("shadowOn", ui.ShadowOn);
	modelShader->unUse();

	/* Game Object */
	screenFBO.update(vw, vh);
	boardManager->update(BoardManager::updateContent{ deltaTime, m_camera.get() });
	speaker.isHover = speaker.bb.raycast(m_camera->Position, m_camera->Front, 10, speaker.m_transform.getModelMatrix());
	speaker.setListenerPosition(m_camera->Position, m_camera->Front);

	/* UI */
	modelShader->use();
	modelShader->setVec3("viewPos", viewPos);
	modelShader->unUse();
	m_dirLight->direction = glm::vec3(ui.dirLightDirection[0], ui.dirLightDirection[1], ui.dirLightDirection[2]);

	if (ui.gameStartBtn) {
		boardManager->setGameStart();
		ui.gameStarting = true;
	}
	switch (boardManager->getGameStatus())
	{
	case BoardManager::GameStatus::End:
		if(boardManager->winnwer == Chess::Color::Black)
			ui.winner = "Black";
		else
			ui.winner = "White";
		ui.gameStarting = false;
		break;
	case BoardManager::GameStatus::Preparing:
		ui.gameStarting = false;
		break;
	case BoardManager::GameStatus::SetBlackHint:
	case BoardManager::GameStatus::WaitForBlack:
	case BoardManager::GameStatus::BlackSeletced:
		ui.gameStarting = true;
		ui.gameStateStr = "White's Turn";
		break;
	case BoardManager::GameStatus::SetWhiteHint:
	case BoardManager::GameStatus::WaitForWhite:
	case BoardManager::GameStatus::WhiteSeletced:
		ui.gameStarting = true;
		ui.gameStateStr = "Black's Turn";
		break;
	}
	
	boardManager->BlackBot = ui.BlackBot;
	boardManager->WhiteBot = ui.WhiteBot;
	boardManager->aiSpeed = ui.botSpeed;
	
	speaker.setVolumn(ui.musicVolumn);
	return false;
}

bool SimpleScene::processInput(GLFWwindow* window)
{
	boardManager->processInput(window);

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

void SimpleScene::frame_buffer_size_callback(GLFWwindow* window, int width, int height)
{
	vw = width;
	vh = height;
	glViewport(0, 0, vw, vh);
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

void SimpleScene::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (!isPause)
		speaker.mouse_button_callback(window, button, action, mods);
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

void SimpleSceneGUI::GameUI()
{ 
	// Main body of the Demo window starts here.
	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);
	
	if (!ImGui::Begin("Game UI"))
	{
		ImGui::End();
		return;
	}
	else {

		if (ImGui::TreeNode("Hint Text"))
		{
			ImGui::Text("Hint: \n%s", str.c_str());
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Volumn"))
		{
			ImGui::SliderFloat("Music Volumn", &musicVolumn, 0.0f, 1.0f, "ratio = %.4f");
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Reversi UI"))
		{
			if (!gameStarting) {
				if (winner.size())
					ImGui::Text("Winner: (%s)", winner);

				if (ImGui::Button("Game Start"))
				{
					gameStartBtn = true;
				}
				else {
					gameStartBtn = false;
				}
			}
			else {
				ImGui::Text("Game Status: (%s)", gameStateStr);

				ImGui::Checkbox("BlackBot", &BlackBot);
				ImGui::Checkbox("WhiteBot", &WhiteBot);
				ImGui::SliderFloat("Bot Speed", &botSpeed, 0.0f, 3.0f, "ratio = %.4f");
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Light and Shadow"))
		{
			ImGui::Text("Directional Light Settings");
			ImGui::SliderFloat3("Position", dirLightPosition, -50.f, 50.f, "ratio = %.4f");
			ImGui::SliderFloat3("Direction", dirLightDirection, -50.f, 50.f, "ratio = %.4f");
			ImGui::NewLine();

			ImGui::Text("Shadow Settings");
			ImGui::Checkbox("Shadow", &ShadowOn);
			ImGui::BeginGroup();
			ImGui::SliderFloat("Bias Max", &maxBias, minBias, 1.f, "ratio = %.5f");
			ImGui::SliderFloat("Bias min", &minBias, 0, maxBias, "ratio = %.5f");
			ImGui::EndGroup();
			ImGui::SliderInt("Sample Num", &sampleNum, 0, 50);
			ImGui::Checkbox("Shadow Debug", &ShadowDebug);
			ImGui::TreePop();
		}

		ImGui::End();
	}

}
