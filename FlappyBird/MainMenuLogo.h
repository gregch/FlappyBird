#pragma once

#include "SceneObject.h"
#include "Texture.h"

class MainMenuLogo: public SceneObject
{
private:
	Texture texture;
	GLfloat	animTime;
	GLuint displayList;
public:
	MainMenuLogo();
	// creates a cursor with an offset for the upperleft point
	MainMenuLogo(const std::string& texturePath);

	void render(Scene &parent);
};