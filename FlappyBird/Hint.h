#pragma once

#include "SceneObject.h"
#include "Texture.h"
#include "Rect.h"

class Hint: public SceneObject
{
private:
	Texture texture;
	float	animTime;
	bool	fadingOut;

public:
	Hint();
	void render(Scene &parent);
	void fadeOut();
private:
	Rect getBoundingRect(SDL_Window *win);
};
