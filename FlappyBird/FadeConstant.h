#pragma once

#include "SceneObject.h"

class FadeConstant: public SceneObject
{
private:
	GLfloat alpha;
public:
	FadeConstant(float alpha = 0.5);
	void render(Scene &parent);
};