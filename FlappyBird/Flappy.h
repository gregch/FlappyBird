#pragma once
#include "SceneObject.h"
#include "Texture.h"
#include "Vector3.h"
#include "Model.h"
#include <SDL_mixer.h>
#include <deque>

class Bridge;

class FlappyTrailPoint
{
public:
	FlappyTrailPoint(float h, float d);

	float height;
	float displacement;
};

class Flappy: public SceneObject
{
public:
	static const Vector3 displacement; // bird displacement from the center
	static const float birdSize;
private:
	static const float initialHeight;
	static const float maxHeight;
	static const float gravity; 
	static const float gravity2; 
	static const float velocityJump;
	static const float maxTrailDistance;

	Bridge *colliderBridge;
	Model  model;
	GLuint texturedDisplayList, solidDisplayList, wireframeDisplayList;
	// positioning
	GLfloat height;
	GLfloat velocity;
	GLfloat acceleration;
	GLfloat acceleration2;
	GLfloat angle;
	// sounds
	Mix_Chunk *flappingSound;
	Mix_Chunk *dieSound;

	bool alive;
	bool jumpedFirstTime;

	// trail
	std::deque<FlappyTrailPoint> trailPoints;
	Texture trailTexture;
	// wings
	float animTime;
public:
	Flappy(Bridge *colliderBridge);

	void kill();
	void respawn();
	bool isDead() const;
	bool heJumpedFirstTime() const;
	Vector3 getPosition() const;
	Bridge *getColliderBridge() const;
	float getHeight() const;

	void render(Scene &parent);
	void handleEvent(Scene &parent, const SDL_Event& ev);
private:
	void drawTrail(Scene &parent);
	void drawBird(RenderMode mode);
};
