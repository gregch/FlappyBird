#pragma once

#include "SceneObject.h"
#include "Vector3.h"

class FadeInOut: public SceneObject
{
private:
	float	animTime;
	float	length;
	Vector3	color;
	float	maxAlpha;
public:
	FadeInOut(float length = 0.75f, float maxAlpha = 1.125, const Vector3& color = Vector3::zero);
	void render(Scene &parent);
	bool fadedOut() const;
};
