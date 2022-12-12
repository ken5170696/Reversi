#pragma once
class SceneStack;
#include <memory>
#include "GameSetting.h"

#include "SceneStack.h"

class Scene
{
public:
	Scene(SceneStack& stack, SceneContext context);
	/*virtual ~State();*/
	virtual void draw() = 0;
	virtual bool update(float deltaTime) = 0;
	virtual bool processInput(GLFWwindow* window) = 0;
	virtual void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) = 0;
	virtual void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) = 0;
	virtual void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) = 0;
	void setSceneID(SceneID::ID _stateID);
	SceneID::ID getSceneID() const;

protected:
	void requestScenePush(SceneID::ID SceneID);
	void requestScenePop();
	void requestSceneClear();
	int getSceneNum() const;
	bool inStack(SceneID::ID SceneID) const;
	SceneContext getContext() const;

private:
	SceneID::ID sceneID;
	SceneStack* _stack;
	SceneContext _context;
};

