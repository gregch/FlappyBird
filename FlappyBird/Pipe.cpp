#include "Pipe.h"
#include "Vector3.h"
#include <GL/freeglut.h>

const float Pipe::aperture = 35.0f;
const float Pipe::upperPipeLength = 400.0f;
const float Pipe::ratio = 8.0f;
const int   Pipe::slices = 8;
const int   Pipe::stacks = 1;
Model		Pipe::pipeEnd;
bool		Pipe::modelsInited = false;


Pipe::Pipe(Bridge& colliderBridge, float initialPosition, float apertureHeight):
	colliderBridge(colliderBridge),
	position(initialPosition + Flappy::displacement.x()),
	apertureHeight(apertureHeight),
	stopped(false)
{
	initModels();

	innerSolidDisplayList = glGenLists(2);
	glNewList (innerSolidDisplayList, GL_COMPILE);
	{
		drawInnerPipe(SOLID_RENDER);
	}
	glEndList ();

	innerWireframeDisplayList = innerSolidDisplayList + 1;
	glNewList (innerWireframeDisplayList, GL_COMPILE);
	{
		drawInnerPipe(WIREFRAME_RENDER);
	}
	glEndList ();
}

void Pipe::clean()
{
	glDeleteLists(innerSolidDisplayList, 2);
}

float Pipe::getAbsolutePosition() const
{
	return position - Flappy::displacement.x();
}

void Pipe::stop()
{
	stopped = true;
}

void Pipe::render(Scene &parent)
{
	float lowerTubeOrigin = colliderBridge.getStreetHeight(position) + apertureHeight - 5;

	float multiplier = parent.app().getOptions()->speedMultiplier();
	float frameTime = parent.app().getFrameTime();

	RenderMode renderMode = parent.app().getOptions()->renderMode();

	glPushAttrib(GL_ENABLE_BIT);
	{
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
		glPushMatrix();
		{
			glTranslate(Vector3::right * position + Vector3::up * (lowerTubeOrigin - ratio));

			if(renderMode == WIREFRAME_RENDER)
				glCallList(innerWireframeDisplayList);
			else 
				glCallList(innerSolidDisplayList);
		}
		glPopMatrix();
	}
	glPopAttrib();

	if(!stopped)
	{
		position -=  frameTime * Bridge::bridgeVelocity * multiplier;
	}
}

bool Pipe::collidesWith(Flappy& flappy) const
{
	float midBirdSize = Flappy::birdSize / 2;

	if(beforePipe())
		return false;
	else if(pastPipe())
		return false;
	else {
		float lowerTubeOrigin = colliderBridge.getStreetHeight(position) + apertureHeight - 5;
		float upperTubeOrigin = lowerTubeOrigin + aperture;
		float height = flappy.getHeight();
		if(height - midBirdSize < lowerTubeOrigin || height + midBirdSize > upperTubeOrigin)
			return true;
		else
			return false;
	}
}

bool Pipe::pastMiddle() const
{
	return position - Flappy::displacement.x()  < Flappy::birdSize / 2;
}

bool Pipe::beforePipe() const
{
	return position - Flappy::displacement.x() - ratio  * 1.1f >  Flappy::birdSize / 2;
}

bool Pipe::pastPipe() const
{
	return position - Flappy::displacement.x()  + ratio * 1.1f < Flappy::birdSize / 2;
}

void Pipe::drawInnerPipe(RenderMode renderMode)
{
	void (_stdcall *cylinderFunc)(GLdouble, GLdouble, GLint, GLint) =
		renderMode == WIREFRAME_RENDER ? glutWireCylinder : glutSolidCylinder;

	glColor(Vector3::fromRGB(82,130,33));
	glMaterial(GL_DIFFUSE, Vector3::fromRGB(82,130,33), 1);
	glMaterial(GL_SPECULAR, Vector3::zero, 0);
	glMaterial(GL_AMBIENT, Vector3::zero, 0);
	glMaterial(GL_EMISSION, Vector3::zero, 0);

	// inner tubes
	glRotate(90, Vector3::right);
	cylinderFunc(ratio, (apertureHeight - ratio), slices, stacks);
	glTranslate(Vector3::backward * (upperPipeLength + aperture + ratio) );
	cylinderFunc(ratio, upperPipeLength - ratio, slices, stacks);
	
	// pipe ends
	glMaterial(GL_AMBIENT, Vector3::fromRGB(82,130,33), 1);
	glRotate(-90, Vector3::right);
	glTranslate(Vector3::down * (upperPipeLength - ratio));

	glPushMatrix();
	{
		glScalef(ratio*1.1f, -ratio * 0.5f ,ratio*1.1f);
		pipeEnd.render(1, renderMode, false);
	}
	glPopMatrix();

	glTranslate(Vector3::down * (aperture + 2*ratio));
	glScalef(ratio*1.1f, ratio * 0.5f ,ratio*1.1f);
	pipeEnd.render(1, renderMode, false);
}

void Pipe::initModels()
{
	if(!modelsInited)
	{
		pipeEnd = Model("pipeEnd.obj");
		modelsInited = true;
	}
}