#pragma once

#include "SceneObject.h"
#include "Model.h"
#include "Camera.h"
#include "Flappy.h"

class Bridge: public SceneObject
{
public:
	static const float bridgeVelocity;
private: 
	static const int nBridges;
	static const float modelLength;

	Model model;
	float animTime;
	GLuint texturedDisplayList, solidDisplayList, wireframeDisplayList;
	bool stopped;
public:
	Bridge();
	void render(Scene &parent);
	void move(float xDisplacement);
	void reset();
	void stop();
	void resume();
	float getStreetHeight(float relativePosition = Flappy::displacement.x()); // retorna la altura de la calle en el momento actual 
private:
	void render(RenderMode mode);
};