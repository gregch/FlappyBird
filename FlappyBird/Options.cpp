#include "Options.h"

Options::Options():
	textures(true),
	wireframe(false),
	gameSpeed(MEDIUM)
{
}

bool Options::getTexturesEnabled() const
{
	return textures;
}

void Options::setTexturesEnabled(bool val)
{
	textures = val;
}

void Options::setShadingModel(bool smooth)
{
	glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);
}

bool Options::getWireframeEnabled() const
{
	return wireframe;
}

void Options::setWireframeEnabled(bool val)
{
	wireframe = val;
}

GameSpeed Options::getGameSpeed() const
{
	return gameSpeed;
}

float Options::speedMultiplier() const
{
	switch (gameSpeed)
	{
	case SLOW:
		return 0.75f;
	case MEDIUM:
		return 1.0f;
	case FAST:
		return 1.25f;
	default:
		return 1.0f;
	}
}

void Options::setGameSpeed(GameSpeed val)
{
	gameSpeed = val;
}

void glToggle(GLenum cap, GLboolean val)
{
	(val ? glEnable : glDisable)(cap);
}

RenderMode Options::renderMode()
{
	if(wireframe)
		return WIREFRAME_RENDER;
	else if(!textures)
		return SOLID_RENDER;
	else
		return TEXTURED_RENDER;
}