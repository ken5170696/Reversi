#include "SceneStack.h"

std::shared_ptr<Scene> SceneStack::createScene(SceneID::ID sceneID)
{
	auto found = sceneFactories.find(sceneID);

	return found->second();
}

void SceneStack::applyPendingChanges()
{
	for (PendingChange change : mPendingList)
	{
		switch (change.action)
		{
		case Push:
			statesStack.push_back(createScene(change.sceneID));
			statesStack.back()->setSceneID(change.sceneID);
			break;
		case Pop:
			statesStack.pop_back();
			break;
		case Clear:
			statesStack.clear();
			break;
		}
	}
	mPendingList.clear();
}
SceneStack::SceneStack(const SceneContext& context)
	: _context(context)
{

}
void SceneStack::update(float deltaTime)
{
	for (auto statesItr = statesStack.begin(); statesItr != statesStack.end(); ++statesItr)
	{
		if (!(*statesItr)->update(deltaTime))
			return;
	}
	applyPendingChanges();
}

void SceneStack::draw()
{
	for (auto statesItr = statesStack.begin(); statesItr != statesStack.end(); ++statesItr)
	{
		(*statesItr)->draw();
	}
}

void SceneStack::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	for (auto statesItr = statesStack.begin(); statesItr != statesStack.end(); ++statesItr)
	{
		(*statesItr)->mouse_callback(window, xposIn, yposIn);
	}
}

void SceneStack::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	for (auto statesItr = statesStack.begin(); statesItr != statesStack.end(); ++statesItr)
	{
		(*statesItr)->scroll_callback(window, xoffset, yoffset);
	}
}

void SceneStack::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	for (auto statesItr = statesStack.begin(); statesItr != statesStack.end(); ++statesItr)
	{
		(*statesItr)->key_callback(window, key, scancode, action, mods);
	}
}

void SceneStack::processInput(GLFWwindow* window)
{
	for (auto statesItr = statesStack.begin(); statesItr != statesStack.end(); ++statesItr)
	{
		if (!(*statesItr)->processInput(window))
			return;
	}

	applyPendingChanges();
}

void SceneStack::pushState(SceneID::ID sceneID)
{
	mPendingList.push_back(PendingChange(Action::Push, sceneID));
}

void SceneStack::popState()
{
	mPendingList.push_back(PendingChange(Pop));
}

void SceneStack::clearStates()
{
	mPendingList.push_back(PendingChange(Clear));
}

bool SceneStack::isEmpty() const
{
	return statesStack.empty();
}

int SceneStack::getStackNum() const
{
	return statesStack.size();
}

bool SceneStack::inStack(SceneID::ID sceneID) const
{
	for (const auto& stackElement : statesStack)
		if (sceneID == stackElement->getSceneID())
			return true;

	return false;
}
