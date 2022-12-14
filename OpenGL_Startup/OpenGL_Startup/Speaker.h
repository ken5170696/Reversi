#pragma once



#include "GameObject.h"
#include "BoundingBox.h"

/* reference: https://www.ambiera.com/irrklang/tutorial-3dsound.html */
#include "irrKlang/irrKlang.h"
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

#define SPEAKER_MODEL_PATH "./resource/model/speaker/BOX_01.obj"

class Speaker : public GameObject
{
public:
	Speaker();
	~Speaker();

	void setListenerPosition(const glm::vec3& pos, const glm::vec3& dir);

	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	void update(float deltaTime);

	void setMusicPlay(bool isPlay);
	void setVolumn(float volumn);
	BoundingBox bb;

	ISoundEngine* engine;
	ISound* music;

	bool isHover;

private:
	bool m_isPlay;
	float m_volumn;
};

