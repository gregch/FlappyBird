#include "Scene.h"
#include "SceneObject.h"
#include "Logger.h"

using namespace std;

Scene::Scene(App &app):
	parent(app)
{
}

App& Scene::app() const
{
	return parent;
}

void Scene::init()
{
}

bool Scene::handleEvent(const SDL_Event&)
{
	return true;
}

void Scene::render()
{
}

void Scene::clean()
{
}

Scene::~Scene()
{
}

void Scene::reshape(int, int)
{
}

void Scene::addObject(SceneObject *object)
{
	objects.push_back(object);
}

void Scene::removeObject(SceneObject *object)
{
	for(std::vector<SceneObject *>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		if(*it == object){
			objects.erase(it);
			return;
		}
	}
}

const std::vector<SceneObject *>& Scene::getObjects()
{
	return objects;
}
