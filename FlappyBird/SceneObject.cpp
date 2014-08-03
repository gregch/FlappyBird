#include "SceneObject.h"
#include <stdexcept>

SceneObject::SceneObject():
	enabled(true)
{
}

bool SceneObject::isEnabled() const
{
	return enabled;
}

void SceneObject::disable()
{
	enabled = false;
	onDisable();
}

void SceneObject::enable()
{
	enabled = true;
	onDisable();
}

void SceneObject::toggle(bool val)
{
	enabled = val;
	enabled ? onEnable() : onDisable();
}

void SceneObject::render(Scene &)
{
}

void SceneObject::onEnable()
{
}

void SceneObject::onDisable()
{
}


void SceneObject::handleEvent(Scene &, const SDL_Event&)
{
}

void SceneObject::clean(Scene &)
{
}

SceneObject::~SceneObject()
{
}

