#include "FlappyFeatherParticleSystem.h"
#include "Bridge.h"

const int FlappyFeatherParticleSystem::maxElements = 70;

const float FlappyFeatherParticleSystemElement::gravity = 70;
const float FlappyFeatherParticleSystemElement::initialVelocityVal = 50;
const float FlappyFeatherParticleSystemElement::maxAngularVelocity = 720;

FlappyFeatherParticleSystem::FlappyFeatherParticleSystem(Flappy& f):
	flappy(f),
	featherTexture("feather.png", false, false, 0)
{
	reset();
}


void FlappyFeatherParticleSystem::reset()
{
	elements.clear();
	Vector3 position = flappy.getPosition();
	for(int i = 0; i < maxElements; ++i){
		elements.push_back(FlappyFeatherParticleSystemElement(position));
	}
	animTime = 0;
}

void FlappyFeatherParticleSystem::render(Scene &parent)
{
	animTime += parent.app().getFrameTime();
	vector<FlappyFeatherParticleSystemElement>::iterator it;
	
	glColor(Vector3::fromRGB(200, 0, 0)); // feather color

	glPushAttrib(GL_ENABLE_BIT);
	{
		glMaterial(GL_DIFFUSE, Vector3::fromRGB(200, 0, 0), 1);
		glMaterial(GL_SPECULAR, Vector3::zero, 0);
		glMaterial(GL_AMBIENT, Vector3::fromRGB(200, 0, 0), 0.5);
		glMaterial(GL_EMISSION, Vector3::zero, 0);

		if(parent.app().getOptions()->renderMode() == WIREFRAME_RENDER)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		if(parent.app().getOptions()->renderMode() == TEXTURED_RENDER)
		{
			glBindTexture(featherTexture);
		}
		int aliveElements = 0;
		for(it = elements.begin(); it != elements.end(); ++it)
		{
			Vector3 position = it->getPosition(animTime);
			if(position.y() > flappy.getColliderBridge()->getStreetHeight())
			{
				drawElement(position, it->getAngle(animTime), it->getRotatingVector());
				++aliveElements;
			}
		}

		if(aliveElements == 0)
		{
			disable();
			reset();
		}
	}
	glPopAttrib();

	
}

void FlappyFeatherParticleSystem::drawElement(const Vector3& position, float angle, const Vector3& rotatingVector)
{
	glPushMatrix();
	{
		glTranslate(position);
		glRotate(angle, rotatingVector);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0, 0); glVertex2f(-2, 2);
			glTexCoord2f(0, 1); glVertex2f(-2, -2);
			glTexCoord2f(1, 1); glVertex2f(2, -2);
			glTexCoord2f(1, 0); glVertex2f(2, 2);
		}
		glEnd();
	}
	glPopMatrix();
}

FlappyFeatherParticleSystemElement::FlappyFeatherParticleSystemElement(const Vector3& initialPosition):
	initialPosition(initialPosition)
{
	initialVelocity = Vector3(
		(((float)rand() / RAND_MAX) - 0.5f) * initialVelocityVal * 2,
		(((float)rand() / RAND_MAX) - 0.5f) * initialVelocityVal * 2,
		(((float)rand() / RAND_MAX) - 0.5f) * initialVelocityVal * 2
	);
	rotatingVector = Vector3::random().normalized();
	angularVelocity = (((float)rand() / RAND_MAX) - 0.5f) * maxAngularVelocity;
}

Vector3 FlappyFeatherParticleSystemElement::getPosition(float animTime) const
{
	return initialPosition + animTime * initialVelocity + animTime * animTime * gravity * Vector3::down;
}

float FlappyFeatherParticleSystemElement::getAngle(float animTime) const
{
	return animTime * angularVelocity;
}

Vector3 FlappyFeatherParticleSystemElement::getRotatingVector() const
{
	return rotatingVector;
}