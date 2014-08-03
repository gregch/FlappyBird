#pragma once

#include "SceneObject.h"
#include "Vector3.h"

class Camera: public SceneObject
{
private:
	static const float minLatitude, maxLatitude;
	static const float minDistance, maxDistance;
	static const float minAzimut, maxAzimut;
	static		 bool  freeLook;

	float	distance, latitude, azimut;
	Vector3 center;
	Vector3 position;
	float	trembleAnimTime;

	bool	canMove;
public:
	Camera(float azimut, float latitude, float distance, const Vector3& center);
	const Vector3& getPosition() const;
	const Vector3& getCenter() const;
	void reshape(int width, int height);
	void handleEvent(Scene &parent, const SDL_Event& ev);
	void render(Scene &parent);
	void tremble(float seconds);

	void disableMove();
	void enableMove();
private:
	void recalculatePosition();
};
