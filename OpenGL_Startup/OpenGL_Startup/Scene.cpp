#include "Scene.h"

Scene::Scene(SceneStack& stack, SceneContext context)
	: _context(context)
{
	_stack = &stack;
}
void Scene::setSceneID(SceneID::ID _sceneID)
{
	sceneID = _sceneID;
}
SceneID::ID Scene::getSceneID() const
{
	return sceneID;
}

void Scene::requestScenePush(SceneID::ID sceneID)
{
	_stack->pushState(sceneID);
}

void Scene::requestScenePop()
{
	_stack->popState();
}

void Scene::requestSceneClear()
{
	_stack->clearStates();
}

int Scene::getSceneNum() const
{
	return _stack->getStackNum();
}

bool Scene::inStack(SceneID::ID sceneID) const
{
	return _stack->inStack(sceneID);
}

SceneContext Scene::getContext() const
{
	return _context;
}