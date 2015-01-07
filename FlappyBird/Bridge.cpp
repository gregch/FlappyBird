#include "Bridge.h"
#include <GL/glu.h>
#include <iostream>

const float Bridge::modelLength = 1245;
const int Bridge::nBridges = 2;
const float Bridge::bridgeVelocity = 30;


Bridge::Bridge():
	model("bridge.dae"),
	animTime(0),
	stopped(false)
{
	texturedDisplayList = glGenLists(3);
	glNewList (texturedDisplayList, GL_COMPILE);
	{
		render(TEXTURED_RENDER);
	}
	glEndList ();
	solidDisplayList = texturedDisplayList+1;
	glNewList (solidDisplayList, GL_COMPILE);
	{
		render(SOLID_RENDER);
	}
	glEndList ();
	wireframeDisplayList = texturedDisplayList+2;
	glNewList (wireframeDisplayList, GL_COMPILE);
	{
		render(WIREFRAME_RENDER);
	}
	glEndList ();
}

void Bridge::render(Scene &parent)
{
	glPushMatrix();
	{
		glTranslate(600 * Vector3::backward);
		glTranslate(9 * Vector3::down);
		glRotate(180, Vector3::up);

		glPushAttrib(GL_ENABLE_BIT);
		{
			glDisable(GL_TEXTURE_2D);

			glTranslate((animTime - modelLength) * Vector3::right);

			switch(parent.app().getOptions()->renderMode())
			{
			case TEXTURED_RENDER:
				glCallList(texturedDisplayList);
				break;
			case SOLID_RENDER:
				glCallList(solidDisplayList);
				break;
			case WIREFRAME_RENDER:
				glCallList(wireframeDisplayList);
				break;
			}
		}
		glPopAttrib();
	}
	glPopMatrix();

	if(!stopped)
	{
		float multiplier = parent.app().getOptions()->speedMultiplier();
		animTime +=  parent.app().getFrameTime() * bridgeVelocity * multiplier;
		if(animTime > modelLength)
			animTime -= modelLength;
	}
}

void Bridge::reset()
{
	animTime = 0;
}

void Bridge::stop()
{
	stopped = true;
}

void Bridge::resume()
{
	stopped = false;
}

float Bridge::getStreetHeight(float relativePosition)
{
	// aproximation to the bridge street curve
	float absolutePosition = (animTime + relativePosition);
	if(absolutePosition > modelLength)
			absolutePosition -= modelLength;
	if(absolutePosition < 0)
		absolutePosition += modelLength;
	return 93.0f - sinf(absolutePosition / modelLength * (float)M_PI) * 11.0f;
}

void Bridge::render(RenderMode mode)
{
	for(int i = 0; i < nBridges; ++ i)
	{
		model.render(6, mode);
		glTranslate(modelLength * Vector3::right);
	}
}
void Bridge::move(float xDisplacement){
	animTime += xDisplacement;
	if(animTime > modelLength)
			animTime -= modelLength;

}