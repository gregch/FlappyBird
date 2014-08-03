#pragma once

#include "Texture.h"
#include "SceneObject.h"
#include <SDL.h>

class Cursor: public SceneObject
{
private:
	Texture texture;
	int offsetX, offsetY;
public:
	Cursor();
	// creates a cursor with an offset for the upperleft point
	Cursor(const std::string& path, int offsetX = 0, int offsetY = 0);
	int getOffsetY();
	void render(Scene &parent);
};