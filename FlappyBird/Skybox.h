#pragma once

#include "SceneObject.h"
#include "Texture.h"
#include "Camera.h"

class Skybox: public SceneObject
{
private:
	Texture texture;
	Camera *camera;
	GLuint texturedDisplayList, solidDisplayList, wireframeDisplayList;
public:
	Skybox(Camera *camera);
	void render(Scene &parent);
	void render(RenderMode mode);
};