#include "MainMenuLogo.h"
#include "Vector3.h"

MainMenuLogo::MainMenuLogo():
	animTime(0), displayList(0)
{
}

MainMenuLogo::MainMenuLogo(const std::string& texturePath):
	texture(texturePath),
	animTime(0)
{
	displayList = glGenLists(1);
	glNewList (displayList, GL_COMPILE);
	{
		glPushAttrib(GL_ENABLE_BIT);
		{
			glDisable(GL_LIGHTING);
			texture.render();
		}
		glPopAttrib();
	}
	glEndList();
}

void MainMenuLogo::render(Scene &parent)
{
	float angle = sinf(animTime * 2 * (GLfloat) M_PI) * 6.0f;
	float offsetDistance = sinf(animTime * 3 * (GLfloat) M_PI) * 0.15f;

	glPushMatrix();
	{
		glLoadIdentity();
		glTranslate(Vector3::up);
		glTranslate(Vector3::backward * (4 + offsetDistance));
		glRotate(angle, Vector3::forward);
		glCallList(displayList);
	}
	glPopMatrix();

	animTime += parent.app().getFrameTime() * 0.05f;
	if(animTime > 6)
		animTime -= 6;
}