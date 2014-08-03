#include "FadeConstant.h"
#include "Vector3.h"


FadeConstant::FadeConstant(float alpha):
	alpha(alpha)
{
}

void FadeConstant::render(Scene &)
{
	glPushAttrib(GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT);
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		{
			glLoadIdentity();
			glOrtho(0, 1, 0, 1, -1, 1);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			{
				glLoadIdentity();
				glColor(Vector3::zero, alpha);
				glBegin(GL_QUADS);
				{
					glVertex2f(0, 0);
					glVertex2f(0, 1);
					glVertex2f(1, 1);
					glVertex2f(1, 0);
				}
				glEnd();
			}
			glPopMatrix();
		}
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}
	glPopAttrib();
}