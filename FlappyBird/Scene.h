#pragma once

#include <SDL.h>
#include "App.h"
#include <string>
#include <vector>

class SceneObject;

class Scene {
private:
	std::vector<SceneObject *> objects;
	App &parent;
public:
	Scene(App& parent);

	App &app() const;
	
	/* child object management */
	void addObject(SceneObject *object);
	void removeObject(SceneObject *object);
	const std::vector<SceneObject *>& getObjects();
	
	// optional methods for child classes
	virtual void init();
	virtual bool handleEvent(const SDL_Event& ev);
	virtual void reshape (int w, int h);
	virtual void render();
	virtual void clean();
	virtual ~Scene();
};