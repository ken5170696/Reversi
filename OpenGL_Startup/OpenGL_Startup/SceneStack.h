#pragma once
class Scene;

#include <utility>
#include <functional>
#include <map>
#include <vector>

#include "Scene.h"

class SceneStack
{
public:
	enum Action
	{
		Push,
		Pop,
		Clear,
	};

private:
	struct PendingChange
	{
		PendingChange(Action _action, SceneID::ID _sceneID) {
			action = _action;
			sceneID = _sceneID;
		};
		PendingChange(Action _action) {
			action = _action;
			sceneID = SceneID::ID::None;
		};
		Action action;
		SceneID::ID sceneID;
	};
private:
	SceneContext _context;
	std::vector<std::shared_ptr<Scene>> statesStack;
	std::vector<PendingChange> mPendingList;
	std::map<SceneID::ID, std::function<std::shared_ptr<Scene>()>> sceneFactories;

public:
	SceneStack(const SceneContext& context);

	void processInput(GLFWwindow* window);
	void update(float deltaTime);
	void draw();

	void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	void pushState(SceneID::ID sceneID);
	void popState();
	void clearStates();
	bool isEmpty() const;
	int getStackNum() const;

	template<typename T>
	void registerScene(SceneID::ID sceneID);

	bool inStack(SceneID::ID sceneID)  const;

private:
	std::shared_ptr<Scene> createScene(SceneID::ID sceneID);
	void applyPendingChanges();
};

template<typename T>
void SceneStack::registerScene(SceneID::ID sceneID)
{
	sceneFactories[sceneID] = [this]()
	{
		return std::shared_ptr<Scene>(new T(*this, _context));
	};
}