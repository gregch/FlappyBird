#include "Lighting.h"

Lighting::Lighting():
	color(Vector3::one),
	direction(Vector3::forward)
{
	applyLighting();
}

void Lighting::setColor(PresetLightColor color)
{
	switch(color)
	{
	case LIGHTCOLOR_WHITE:
		setColor(Vector3::one);
		break;
	case LIGHTCOLOR_RED:
		setColor(Vector3(1,0,0));
		break;
	case LIGHTCOLOR_PSICODELIC:
		setColor(Vector3::random());
		break;
	case LIGHTCOLOR_BLACK:
		setColor(Vector3::zero);
		break;
	}
}

void Lighting::setColor(const Vector3& val)
{
	color = val;
	applyLighting();
}

void Lighting::setDirection(PresetLightDirection direction)
{
	switch(direction)
	{
	case LIGHTDIRECTION_BACK:
		setDirection(Vector3::backward);
		break;
	case LIGHTDIRECTION_FRONT:
		setDirection(Vector3::forward);
		break;
	case LIGHTDIRECTION_RIGHT:
		setDirection(Vector3::right);
		break;
	case  LIGHTDIRECTION_UP:
		setDirection(Vector3::up);
		break;
	}
}


void Lighting::setDirection(const Vector3& val)
{
	direction = val;
	applyLighting();
}

void Lighting::applyLighting()
{
	glToggle(GL_LIGHTING, color != Vector3::zero);
	glToggle(GL_LIGHT0, color != Vector3::zero);    
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glToggle(GL_NORMALIZE, color != Vector3::zero);
	glLight(GL_LIGHT1, GL_AMBIENT, color * 0.5f, 1);
	glLight(GL_LIGHT1, GL_DIFFUSE, color, 1);
	glLight(GL_LIGHT0, GL_POSITION, direction, 0);
	glToggle(GL_LIGHT1, color != Vector3::zero);
}