#pragma once
#include "SceneObject.h"
#include "Texture.h"
#include "Bridge.h"
#include "Flappy.h"


class Pipe: public SceneObject
{
private:
	// 
	static Model pipeEnd;
	static bool  modelsInited;
	
	static const float aperture;
	static const float upperPipeLength;
	static const float ratio;
	static const int  slices;
	static const int stacks;
	
	int innerSolidDisplayList, innerWireframeDisplayList;
	float apertureHeight; // y position of aperture
	float position; // x position relative to flappy
	Bridge &colliderBridge;
	bool stopped;
public:
	Pipe(Bridge& colliderBridge, float initialPosition, float apertureHeight);

	float getAbsolutePosition() const;
	void render(Scene &parent);
	void stop();
	bool collidesWith(Flappy& flappy) const;
	bool pastPipe() const;
	bool beforePipe() const;
	bool pastMiddle() const;

	void clean();
private:
	static void initModels();
	void drawInnerPipe(RenderMode mode);
};