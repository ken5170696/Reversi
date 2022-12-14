#include "Speaker.h"

Speaker::Speaker()
	: GameObject(SPEAKER_MODEL_PATH, glm::vec3(0.0, -0.5, 1.2))
{
	m_transform.scale = glm::vec3(0.5f, 0.5f, 0.5f);
	bb = BoundingBox(this->m_model->meshes);

	engine = createIrrKlangDevice();

	music = engine->play3D("./resource/audio/love.mp3", 
		vec3df(m_transform.position.x, m_transform.position.y, m_transform.position.z), 
		true, false, true
	);

	if (music) music->setMinDistance(1.5f);

	m_isPlay = true;
}

Speaker::~Speaker()
{
	delete engine;
	delete music;
}

void Speaker::setListenerPosition(const glm::vec3& pos, const glm::vec3& dir)
{
	engine->setListenerPosition(
		vec3df(pos.x, pos.y, pos.z),
		vec3df(-dir.x, dir.y, -dir.z)
	);
}

void Speaker::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		if (isHover) {
			if (m_isPlay) setMusicPlay(false);
			else setMusicPlay(true);
		}
	}
}

void Speaker::update(float deltaTime)
{
	
}

void Speaker::setMusicPlay(bool isPlay)
{
	m_isPlay = isPlay;
	if (!isPlay) {
		music->setIsPaused(true);
	}
	else {
		music->setIsPaused(false);
	}
}

void Speaker::setVolumn(float volumn)
{
	if (volumn != m_volumn) {
		m_volumn = volumn;
		music->setVolume(volumn);
	}
}
