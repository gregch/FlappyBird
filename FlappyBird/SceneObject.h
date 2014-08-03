#pragma once

#include "Scene.h"

// Interface que cumplen los objetos que son hijos de un Scene
class SceneObject 
{
private:
	bool enabled;
protected: // para no permitir crear SceneObjects explicitamente
	SceneObject();
public:
	bool isEnabled() const;
	void disable();
	void enable();
	void toggle(bool enabled);

	virtual void render(Scene &parent);
	virtual void handleEvent(Scene &parent, const SDL_Event& ev);
	virtual void clean(Scene &parent);
	virtual void onEnable();
	virtual void onDisable();
	virtual ~SceneObject();
};