#pragma once

#include "SceneObject.h"
#include "Vector3.h"

enum PresetLightDirection { LIGHTDIRECTION_FRONT = 0, LIGHTDIRECTION_UP, LIGHTDIRECTION_RIGHT, LIGHTDIRECTION_BACK }; 
enum PresetLightColor { LIGHTCOLOR_WHITE = 0, LIGHTCOLOR_RED, LIGHTCOLOR_PSICODELIC, LIGHTCOLOR_BLACK }; 

class Lighting
{
private:
	Vector3 direction;
	Vector3 color;
public:
	Lighting();

	void setColor(PresetLightColor color);
	void setColor(const Vector3& color);
	void setDirection(PresetLightDirection direction);
	void setDirection(const Vector3& direction);
	void applyLighting();
};