#pragma once

#include "SceneObject.h"
#include "Vector3.h"
#include "Flappy.h"

class FlappyFeatherParticleSystemElement
{
private:
	static const float gravity;
	static const float initialVelocityVal;
	static const float maxAngularVelocity;

	Vector3 initialPosition;
	Vector3 initialVelocity;
	Vector3 rotatingVector;
	float angularVelocity;
public:
	FlappyFeatherParticleSystemElement(const Vector3& initialPosition);
	Vector3 getPosition(float animTime) const;
	float getAngle(float animTime) const;
	Vector3 getRotatingVector() const;
};

class FlappyFeatherParticleSystem: public SceneObject
{
private:
	static const int maxElements;

	Flappy &flappy;
	std::vector<FlappyFeatherParticleSystemElement> elements;
	Texture featherTexture;
	float animTime;
public:
	FlappyFeatherParticleSystem(Flappy& flappy);
	void render(Scene &parent);
	void reset();
private:
	void drawElement(const Vector3& position, float angle, const Vector3& rotatingVector);
};